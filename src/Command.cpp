/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:16:51 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 22:59:31 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>

void Server::executeUser(Client *sender, std::vector<std::string>)
{
	std::string message = "NickName -> " + sender->getNickname() + "\n";
	send(sender->getFd(), message.c_str(), message.size(), 0);
	message = "fd -> " + itoa(sender->getFd()) + "\n";
	send(sender->getFd(), message.c_str(), message.size(), 0);
}

void Server::executeNick(Client *client, std::vector<std::string>)
{
	std::cout << "NICK : " << client->getFd() << std::endl;
}

void Server::executePrivmsg(Client *sender, std::vector<std::string> command)
{
	if (command.size() < 3)
	{
		std::string error_message = "Usage : PRIVMSG <recipient> :<message>\n";
		send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
		return ;
	}

	std::string recipient = command[1];
	std::string message   = command[2];
	
	if (_clients_list.find(recipient) != _clients_list.end())
	{
		Client* recipient_client = _clients_list[recipient];
		std::string full_message = sender->getNickname() + " : " + message;
		send(recipient_client->getFd(), full_message.c_str(), full_message.size(), 0);
	}
	else 
	{
		std::string error_message = "Client not found\n";
		send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
		return ;
	}
}

void Server::executeQuit(Client *client, std::vector<std::string>)
{
	close(client->getFd());
	for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end();)
	{
		it->fd == client->getFd() ? it = _poll_fds.erase(it) : it++;
	}
	_clients_list.erase(itoa(client->getFd()));
	delete client;

	std::cout << "Client deconnecte" << std::endl;
}
