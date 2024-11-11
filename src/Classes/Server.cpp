#include "Server.hpp"
#include <IrcError.hpp>
#include <csignal>
#include <netinet/in.h>
#include <sha256.h>

extern sig_atomic_t server_running;

/* constructor  */

Server::Server(const std::string &port, const std::string &password)
    : _port(atoi(port.c_str())),
      _password(password),
      _command(this)
{
    sha256(reinterpret_cast<const uint8_t *>(password.c_str()), password.size(), _hash);
    _createSocket();
    _bindSocket();
    _listenSocket();
    _newFdToPoll();
}

/* destructor */

Server::~Server(void)
{
    close(_server_fd);

    for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); it++)
        close(it->fd);

    for (ClientMap::iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
        delete it->second;

    for (ChannelMap::iterator it = _channels_list.begin(); it != _channels_list.end(); it++)
        delete it->second;
}

std::string         Server::getName() const { return _server_name; }
int                 Server::getClientCount() const { return _clients_list.size(); }
ChannelMap          Server::getChannelsList(void) const { return (_channels_list); }
ClientMap           Server::getClientsList(void) const { return (_clients_list); }
std::vector<pollfd> Server::getPollFds(void) const { return (_poll_fds); }

int Server::getClientFdByNickname(const std::string &nickname) const
{
    for (ClientMap::const_iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return it->first;
    }
    return 0;
}

Client *Server::getClientByNickname(const std::string &nickname) const
{
    for (ClientMap::const_iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return it->second;
    }
    return NULL;
}

Channel *Server::getChannelByChannelname(const std::string &channelname) { return _channels_list[channelname]; }

void Server::addChannel(const std::string &channel_name, Channel *channel) { _channels_list[channel_name] = channel; }
void Server::addClient(int fd, Client *client) { _clients_list[fd] = client; }

void Server::delClient(int fd)
{
    for (std::vector<pollfd>::iterator poll_it = _poll_fds.begin(); poll_it != _poll_fds.end();)
        poll_it->fd == fd ? poll_it = _poll_fds.erase(poll_it) : poll_it++;

    delete _clients_list[fd];
    _clients_list.erase(fd);
    close(fd);
}

void Server::broadcastServer(const std::string &message)
{
    for (ClientMap::iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
        send(it->first, message.c_str(), message.length(), 0);
}

void Server::run()
{
    while (server_running)
    {
        int poll_count = poll(_poll_fds.data(), _poll_fds.size(), -1);
        if (poll_count == -1)
            break;

        handleEvents();
    }
}

void Server::handleEvents()
{
    if (_poll_fds[0].revents & POLLIN)
        acceptNewClient();

    for (size_t i = 1; i < _poll_fds.size(); i++)
    {
        if (_poll_fds[i].revents & POLLIN)
            handleCommand(_poll_fds[i].fd);
    }
}

void Server::acceptNewClient()
{
    sockaddr_in client_address;
    socklen_t   client_len = sizeof(client_address);

    int opt = 1;
    if (setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        throw IrcError("Impossible to set socket options", SERVER_INIT);

    int client_fd = accept(_server_fd, (sockaddr *)&client_address, &client_len);
    if (client_fd < 0)
        return;

    _clients_list[client_fd] = new Client(client_fd);

    pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    client_pollfd.revents = 0;
    _poll_fds.push_back(client_pollfd);
}

void Server::handleCommand(int client_fd)
{
    char buffer[1024] = {0};
    int  valread = read(client_fd, buffer, sizeof(buffer) - 1);
    if (valread == 0)
    {
        delClient(client_fd);
        return;
    } else if (valread < 0)
        return;

    Client *client = _clients_list[client_fd];
    client->appendToBuffer(std::string(buffer, valread));

    std::size_t pos;
    while ((pos = client->getBuffer().find('\n')) != std::string::npos)
    {
        std::string commandLine = client->getBuffer().substr(0, pos);
        client->clearBufferUpTo(0, pos);
        std::vector<std::string> command = client->splitCommand(commandLine);

        if (command[0].empty())
            continue;
        if (!client->getIsAuthenticated())
        {
            if (command[0] != "PASS" && commandLine != "CAP LS 302\r")
            {
                IrcError e(client->getNickname(), CLIENT_NOTREGISTERED);
                e.sendto(*client);
				return;
            }
        } else if (!client->getIsRegistered())
        {
            if (command[0] != "NICK" && command[0] != "USER")
            {
                IrcError e(client->getNickname(), CLIENT_NOTREGISTERED);
                e.sendto(*client);
				return;
            }
        }
		if (commandLine != "CAP LS 302\r")
            _command.exec(command[0], client, command);
    }
}

/* private function */

void Server::_initSockAddr(sockaddr_in &address)
{
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
}

void Server::_newFdToPoll()
{
    pollfd server_pollfd;
    server_pollfd.fd = this->_server_fd;
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    this->_poll_fds.push_back(server_pollfd);
}

void Server::_createSocket(void)
{
    this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_server_fd == -1)
        throw IrcError("Impossible to create the server socket", SERVER_INIT);
}

void Server::_bindSocket(void)
{
    sockaddr_in address;
    _initSockAddr(address);

    if (bind(this->_server_fd, (sockaddr *)&address, sizeof(address)) < 0)
        throw IrcError("Impossible to link socket", SERVER_INIT);
}

void Server::_listenSocket()
{
    if (listen(this->_server_fd, 5) > 0)
        throw IrcError("Impossible to listen on the socket", SERVER_INIT);
}

__attribute((__annotate__(("fla")))) std::string Server::getPassword() const { return _password; }
__attribute((__annotate__(("fla")))) bool Server::checkPassword(const std::string &password) const
{
    if (password.empty())
        return false;
    uint8_t hash[32];
    sha256(reinterpret_cast<const uint8_t *>(password.c_str()), password.size(), hash);
    return std::memcmp(_hash, hash, 32) == 0;
}

std::string Server::getCreationDate() const { return _creationDate; }
