/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:43:08 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/20 21:07:24 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include "Server.hpp"

Server::Server(char *port) : port(atoi(port))
{
   
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		std::cerr << "Erreur: impossible de créer le socket serveur" << std::endl;
		exit(EXIT_FAILURE);
	}
   
	sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(port));

	if (bind(server_fd, (sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Erreur: impossible de lier le socket" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 5) < 0)
	{
		std::cerr << "Erreur: impossible de mettre le socket en écoute" << std::endl;
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	pollfd server_pollfd;
	server_pollfd.fd = server_fd;
	server_pollfd.events = POLLIN; 
	poll_fds.push_back(server_pollfd);
}


void Server::run()
{
	std::cout << "Le serveur IRC est prêt et écoute sur le port " << port << std::endl;
	while (true)
	{
		int poll_count = poll(poll_fds.data(), poll_fds.size(), -1); 
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
   
	if (poll_fds[0].revents & POLLIN)
		acceptNewClient();

	for (size_t i = 1; i < poll_fds.size(); ++i)
	{
		if (poll_fds[i].revents & POLLIN)
			handleClientMessage(poll_fds[i].fd);
	}
}

void Server::acceptNewClient()
{
	sockaddr_in client_address;
	socklen_t client_len = sizeof(client_address);
	int client_fd = accept(server_fd, (sockaddr *)&client_address, &client_len);
	if (client_fd < 0)
	{
		std::cerr << "Erreur: impossible d'accepter une connexion" << std::endl;
		return;
	}

	std::cout << "Nouvelle connexion : " << inet_ntoa(client_address.sin_addr) << std::endl;

	Client new_client(client_fd);
	clients[client_fd] = new_client;

	pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	poll_fds.push_back(client_pollfd);
}

void Server::handleClientMessage(int client_fd)
{
	char buffer[1024] = {0};
	int valread = read(client_fd, buffer, 1024);

	if (valread > 0)
	{
		std::string received_message(buffer);
		std::cout << "Message reçu : " << received_message;
	}
	else
	{   
		close(client_fd);
		for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end();)
			it->fd == client_fd ? it = poll_fds.erase(it) : it++;
		clients.erase(client_fd);
		std::cout << "Client déconnecté" << std::endl;
	}
}
