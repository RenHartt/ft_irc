/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:43:08 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/21 14:11:20 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>

#include "Server.hpp"

Server::Server(int port)
{
   
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
   
	sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	bind(this->_server_fd, (sockaddr *)&address, sizeof(address));
	listen(this->_server_fd, 5);

	pollfd server_pollfd;
	server_pollfd.fd = this->_server_fd;
	server_pollfd.events = POLLIN; 
	this->_poll_fds.push_back(server_pollfd);
}


void Server::run()
{
	while (true)
	{
		poll(this->_poll_fds.data(), this->_poll_fds.size(), -1); 
		handleEvents(); 
	}
}

void Server::handleEvents()
{
   
	if (this->_poll_fds[0].revents & POLLIN)
		acceptNewClient();

	for (size_t i = 1; i < this->_poll_fds.size(); ++i)
	{
		if (this->_poll_fds[i].revents & POLLIN)
			handleClientMessage(this->_poll_fds[i].fd);
	}
}

void Server::acceptNewClient()
{
	sockaddr_in client_address;
	socklen_t client_len = sizeof(client_address);
	int client_fd = accept(this->_server_fd, (sockaddr *)&client_address, &client_len);

	Client new_client;
	_clients[client_fd] = new_client;

	pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	this->_poll_fds.push_back(client_pollfd);
}

void Server::handleClientMessage(int client_fd)
{
	char buffer[1024] = {0};
	int valread = read(client_fd, buffer, 1024);

	if (valread > 0) std::cout << buffer;
	else
	{   
		close(client_fd);
		for (std::vector<pollfd>::iterator it = this->_poll_fds.begin(); it != this->_poll_fds.end();)
			it->fd == client_fd ? it = this->_poll_fds.erase(it) : it++;
		_clients.erase(client_fd);
	}
}
