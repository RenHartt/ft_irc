/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:30:19 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/26 19:47:29 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <netinet/in.h>

bool running = true;

/* constructor  */

Server::Server(const std::string &port, const std::string &password)
    : _port(atoi(port.c_str())),
      _password(password),
      _command(this)
{
    _createSocket();
    _bindSocket();
    _listenSocket();
    _newFdToPoll();
}

/* destructor */

Server::~Server(void)
{
    close(_server_fd);

    for (std::vector<pollfd>::iterator it = _poll_fds.begin();
         it != _poll_fds.end(); it++)
        close(it->fd);

    for (ClientMap::iterator it = _clients_list.begin();
         it != _clients_list.end(); it++)
        delete it->second;

    for (ChannelMap::iterator it = _channels_list.begin();
         it != _channels_list.end(); it++)
        delete it->second;
}

/* getter */

ChannelMap Server::getChannelsList(void) const
{
    return (_channels_list);
}

ClientMap Server::getClientsList(void) const
{
    return (_clients_list);
}

std::vector<pollfd> Server::getPollFds(void) const { return (_poll_fds); }

/* adder */

void Server::addChannel(const std::string &channel_name, Channel *channel)
{
    _channels_list[channel_name] = channel;
}

void Server::addClient(int fd, Client *client) { _clients_list[fd] = client; }

void Server::run()
{
    std::cout << "The IRC server is listening on port :" << _port << std::endl;
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
    {
        acceptNewClient();
    }

    for (size_t i = 1; i < _poll_fds.size(); i++)
    {
        if (_poll_fds[i].revents & POLLIN)
        {
            handleCommand(_poll_fds[i].fd);
        }
    }
}

void Server::acceptNewClient()
{
    sockaddr_in client_address;
    socklen_t   client_len = sizeof(client_address);

    int client_fd =
        accept(_server_fd, (sockaddr *)&client_address, &client_len);
    if (client_fd < 0)
    {
        std::cerr << "Erreur: impossible d'accepter une connexion" << std::endl;
        return;
    }

    Client *new_client = new Client(client_fd);
    _clients_list[client_fd] = new_client;
    std::cout << "Nouvelle connexion : " << inet_ntoa(client_address.sin_addr)
              << std::endl;

    pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    client_pollfd.revents = 0;
    _poll_fds.push_back(client_pollfd);
}

void Server::handleCommand(int client_fd)
{
    Client                  *client = _clients_list[client_fd];
    char                     buffer[1024] = {0};
    int                      valread = read(client_fd, buffer, 1024);
    std::vector<std::string> command = splitCommand(buffer);
    if (valread >= 1 && !command.empty())
        _command.exec(command[0], client, command);
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
    {
        throw IrcError("Impossible to create the server socket", SERVER);
    }
}

void Server::_bindSocket(void)
{
    sockaddr_in address;
    _initSockAddr(address);

    if (bind(this->_server_fd, (sockaddr *)&address, sizeof(address)) < 0)
    {
        throw IrcError("Impossible to link socket", SERVER);
    }
}

void Server::_listenSocket()
{
    if (listen(this->_server_fd, 5) < 0)
    {
        throw IrcError("Impossible to listen on the socket", SERVER);
    }
}
