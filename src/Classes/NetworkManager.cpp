#include <IrcError.hpp>
#include <NetworkManager.hpp>
#include <cstdlib>
#include <cstring>
#include <sys/poll.h>
#include <unistd.h>

NetworkManager::NetworkManager(const std::string &port) : _port(atoi(port.c_str())) {}

NetworkManager::NetworkManager(const int port) : _port(port) {}

NetworkManager::NetworkManager(void) : _port(PORT_NOT_SET) {}

NetworkManager::~NetworkManager() { close(_fd); }

void NetworkManager::createSocket(void)
{
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_fd == -1)
    {
        throw IrcError("Impossible to create the server socket", SERVER_INIT);
    }
}

void NetworkManager::bindSocket(void)
{
    sockaddr_in address;
    _initSockAddr(address);

    if (bind(this->_fd, (sockaddr *)&address, sizeof(address)) < 0)
    {
        throw IrcError("Impossible to link socket", SERVER_INIT);
    }
}

void NetworkManager::listenSocket()
{
    if (listen(this->_fd, 5) > 0)
    {
        throw IrcError("Impossible to listen on the socket", SERVER_INIT);
    }
}

void NetworkManager::_initSockAddr(sockaddr_in &address)
{
	if (_port == PORT_NOT_SET)
        throw IrcError("Port is not set", SERVER_INIT);
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
}

int NetworkManager::getFd() const { return _fd; }

int NetworkManager::getPort() const { return _port; }

void NetworkManager::setPort(const std::string &port) { _port = atoi(port.c_str()); }

void NetworkManager::setPort(const int port) { _port = port; }
