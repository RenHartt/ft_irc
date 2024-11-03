#include "Server.hpp"
#include "bots/BotPluginRegistry.hpp"
#include <IrcError.hpp>
#include <iostream>
#include <netinet/in.h>

bool running = true;

/* constructor  */

Server::Server(const std::string &port, const std::string &password)
    : _network_manager(port),
      _password(password),
      _command(this)
{
    _network_manager.createSocket();
    _network_manager.bindSocket();
    _network_manager.listenSocket();
    _newFdToPoll();

	BotPluginRegistry::plugin_instance().runBots(port, password);

	sha256(reinterpret_cast<const uint8_t*>(password.c_str()), password.size(), _hash);
}


/* destructor */

Server::~Server(void)
{
    for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); it++)
        close(it->fd);

    for (ClientMap::iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
        delete it->second;

    for (ChannelMap::iterator it = _channels_list.begin(); it != _channels_list.end(); it++)
        delete it->second;
}

/* getter */

std::string Server::getName() const { return _server_name; }
int         Server::getClientCount() const { return _clients_list.size(); }
ChannelMap  Server::getChannelsList(void) const { return (_channels_list); }

ClientMap Server::getClientsList(void) const { return (_clients_list); }

Client *Server::getClientByNickname(const std::string &nickname)
{
    for (ClientMap::iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return it->second;
    }
    return NULL;
}

int Server::getFdByNickname(const std::string &nickname)
{
    for (ClientMap::iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return it->first;
    }
    return -1;
}

std::vector<pollfd> Server::getPollFds(void) const { return (_poll_fds); }

void Server::addChannel(const std::string &channel_name, Channel *channel)
{
    _channels_list[channel_name] = channel;
}

void Server::addClient(int fd, Client *client) { _clients_list[fd] = client; }

void Server::run()
{

    std::cout << "The IRC server is listening on port : " << _network_manager.getPort() << std::endl;
    while (running)
    {
        int poll_count = poll(_poll_fds.data(), _poll_fds.size(), -1);
        if (poll_count == -1)
        {
            std::cerr << "Erreur: echec de poll()" << std::endl;
            break;
        }

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
	if (setsockopt(_network_manager.getFd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		throw IrcError("Impossible to set socket options", SERVER_INIT);
	}
    int client_fd = accept(_network_manager.getFd(), (sockaddr *)&client_address, &client_len);
    if (client_fd < 0)
    {
        std::cerr << "Erreur: impossible d'accepter une connexion" << std::endl;
        return;
    }

    Client *new_client = new Client(client_fd);
    _clients_list[client_fd] = new_client;
    std::cout << "Nouvelle connexion : " << inet_ntoa(client_address.sin_addr) << std::endl;

    pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    client_pollfd.revents = 0;
    _poll_fds.push_back(client_pollfd);
}


void Server::handleCommand(int client_fd)
{
    Client *client = _clients_list[client_fd];

    char buffer[1024] = {0};
    int valread = read(client_fd, buffer, 1024);
    if (valread <= 0)
    {
        return;
    }

    std::vector<std::string> command = splitCommand(buffer);
	if (command.empty())
		return;
    if (!client->isAuthenticated() && command[0] != "NICK" && command[0] != "USER" && command[0] != "PASS")
    {
        IrcError e(client->getNickname(), CLIENT_NOTREGISTERED);
        e.sendto(*client);
        return;
    }

    if (!command.empty())
        _command.exec(command[0], client, command);
}


void Server::removeClient(int fd)
{
    ClientMap::iterator it = _clients_list.find(fd);
    if (it != _clients_list.end())
        _clients_list.erase(it);
}
__attribute((__annotate__(("fla"))))
std::string Server::getPassword() { return _password; }

__attribute((__annotate__(("fla"))))
bool Server::checkPassword(const std::string &password) const {
	if (password.empty())
		return false;
    uint8_t hash[32];
    sha256(reinterpret_cast<const uint8_t*>(password.c_str()), password.size(), hash);
    return std::memcmp(_hash, hash, 32) == 0;  
}

void Server::_newFdToPoll()
{
    pollfd server_pollfd;
    server_pollfd.fd = _network_manager.getFd();
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    this->_poll_fds.push_back(server_pollfd);
}

