/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:57:47 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/21 14:09:46 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>

#include "Client.hpp"

class Server
{
	public:
		Server(int port);
		   
		void run();
		void handleEvents();
		void acceptNewClient();
		void handleClientMessage(int client_fd);
	
	private:
		int						_server_fd;
		std::vector<pollfd>		_poll_fds; 
		std::map<int, Client>	_clients;
};
