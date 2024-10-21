/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:43:08 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/21 22:54:55 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>

#include "Server.hpp"

Server::Server(int port): _port(port)
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1)
	{
		std::cerr << "Erreur: impossible de créer le socket serveur" << std::endl;
		exit(EXIT_FAILURE);
	}
   
	sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(_server_fd, (sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Erreur: impossible de lier le socket" << std::endl;
		close(_server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(_server_fd, 5) < 0)
	{
		std::cerr << "Erreur: impossible de mettre le socket en écoute" << std::endl;
		close(_server_fd);
		exit(EXIT_FAILURE);
	}

	pollfd server_pollfd;
	server_pollfd.fd = _server_fd;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	_poll_fds.push_back(server_pollfd);
}

void Server::executeUser(int client_fd)
{
	std::cout << "USER : " << client_fd << std::endl;
}

void Server::executeNick(int client_fd)
{
	std::cout << "NICK : " << client_fd << std::endl;
}

void Server::executePrivmsg(int client_fd)
{
	std::cout << "PRIVMSG : " << client_fd << std::endl;
}

void Server::executeQuit(int client_fd)
{
	deleteClient(client_fd);
}

void Server::init()
{
	_commands["USER"] = &Server::executeUser;
	_commands["NICK"] = &Server::executeNick;
	_commands["PRIVMSG"] = &Server::executePrivmsg;
	_commands["QUIT"] = &Server::executeQuit;
}

void Server::run()
{
	std::cout << "Le serveur IRC est prêt et écoute sur le port " << _port << std::endl;
	while (true)
	{
		int poll_count = poll(_poll_fds.data(), _poll_fds.size(), -1); 
		if (poll_count == -1)
		{
			std::cerr << "Erreur: échec de poll()" << std::endl;
			break;
		}

		handleEvents(); 
	}
}

void Server::handleEvents()
{
   
	if (_poll_fds[0].revents & POLLIN)
		acceptNewClient();

	for (size_t i = 1; i < _poll_fds.size(); ++i)
	{
		if (_poll_fds[i].revents & POLLIN)
			handleCommand(_poll_fds[i].fd);
	}
}

void Server::acceptNewClient()
{
	sockaddr_in client_address;
	socklen_t client_len = sizeof(client_address);
	int client_fd = accept(_server_fd, (sockaddr *)&client_address, &client_len);
	if (client_fd < 0)
	{
		std::cerr << "Erreur: impossible d'accepter une connexion" << std::endl;
		return;
	}

	std::cout << "Nouvelle connexion : " << inet_ntoa(client_address.sin_addr) << std::endl;

	Client new_client;
	_clients[client_fd] = new_client;

	pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	_poll_fds.push_back(client_pollfd);
}

void Server::deleteClient(int client_fd)
{
	close(client_fd);
	for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end();)
	{
		if (it->fd == client_fd)
			it = _poll_fds.erase(it);
		else
			it++;
	}
	_clients.erase(client_fd);
	std::cout << "Client déconnecté" << std::endl;
}

bool Server::isCommand(const std::string &command)
{
	return (command == "USER" ||
			command == "NICK" ||
			command == "PRIVMSG" ||
			command == "QUIT");
}

std::vector<std::string> Server::splitCommand(const char *buffer)
{
    std::vector<std::string> splited;
    std::string command(buffer);
    std::size_t colonPos = command.find(':');
    std::string trailing;
    if (colonPos != std::string::npos)
    {
        trailing = command.substr(colonPos);
        command = command.substr(0, colonPos);
    }
    std::stringstream ss(command);
    std::string token;
    while (ss >> token)
        splited.push_back(token);
    if (!trailing.empty())
        splited.push_back(trailing);
    return (splited);
}

void Server::handleCommand(int client_fd)
{
	char buffer[1024] = {0};
	int valread = read(client_fd, buffer, 1024);
	std::vector<std::string> command = splitCommand(buffer);
	if (valread > 1 && isCommand(command[0]))
	{
		std::map<std::string, void (Server::*)(int)>::iterator it = _commands.find(command[0]);
		(this->*it->second)(client_fd);
	}
	else if (valread > 1)
	{
		std::cout << "Wrong command" << std::endl;
	}
}
