/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:57:47 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 17:15:49 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>

#include "Client.hpp"

class Server
{
	public:
		Server(int port);
	
		void init();
		void run();
		void handleEvents();
		void acceptNewClient();
		void deleteClient(int client_fd);
		void handleCommand(int client_fd);

		std::vector<std::string> splitCommand(const char *buffer);
		bool isCommand(const std::string &command);
	
		void executeUser(int client_fd);
		void executeNick(int client_fd);
		void executePrivmsg(int client_fd);
		void executeQuit(int client_fd);

	private:
		int _port;
		int _server_fd;
		std::vector<pollfd> _poll_fds; 
		std::map<int, Client> _clients;
		std::map<std::string, void (Server::*)(int)> _commands;
};
