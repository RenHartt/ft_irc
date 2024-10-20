/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:57:47 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/20 21:00:18 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <cstring>
#include <cstdlib>
#include "Channel.hpp"

class Server
{
	private:
		int port, server_fd;
		std::vector<pollfd> poll_fds; 
		std::map<int, Client> clients;
		std::map<std::string, Channel> channels;

	public:
		Server(char *port);
		   
		void run();
		void handleEvents();
		void acceptNewClient();
		void handleClientMessage(int client_fd);
};
