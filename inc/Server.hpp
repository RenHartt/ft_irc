/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:57:47 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 22:23:33 by bgoron           ###   ########.fr       */
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
#include "Channel.hpp"

class Client;
class Channel;

class Server
{
	public:
		Server(std::string port, std::string password);

		bool isCommand(const std::string &command);
		std::vector<std::string> splitCommand(const char *buffer);

		void init();
		void run();
		void handleEvents();
		void acceptNewClient();
		void handleCommand(int client_fd);

		void executeUser(Client *client, std::vector<std::string>);
		void executeNick(Client *client, std::vector<std::string>);
		void executePrivmsg(Client *client, std::vector<std::string>);
		void executeQuit(Client *client, std::vector<std::string>);

	private:
		int					_server_fd;
		const int			_port;
		std::string			_password;
		std::string			_server_name;
		std::vector<pollfd>	_poll_fds;

		std::map<std::string, Client *>		_clients_list;
		std::map<std::string, Channel *>	_channels_list;

		std::map<std::string, void (Server::*)(Client *, std::vector<std::string>)>	_commands;
};

std::string itoa(int value);
