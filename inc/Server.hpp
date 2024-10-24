/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:57:47 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 19:30:53 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcError.hpp"

class Client;
class Channel;

class Server
{
  public:
    Server(std::string port, std::string password);
    ~Server(void);

    std::vector<std::string> splitCommand(const char *buffer);

    void run();
    void handleEvents();
    void acceptNewClient();
    void handleCommand(int client_fd);

    std::map<std::string, Client *> getClientsList(void) const;
    std::vector<pollfd>             getPollFds(void) const;

  private:
    int                 _server_fd;
    const int           _port;
    std::string         _password;
    std::string         _server_name;
    std::vector<pollfd> _poll_fds;

    std::map<std::string, Client *>  _clients_list;
    std::map<std::string, Channel *> _channels_list;


	void _createSocket(void);
	void _bindSocket(sockaddr_in &address);
	void _listenSocket(void);

    Command _command;
};

std::string itoa(int value);
