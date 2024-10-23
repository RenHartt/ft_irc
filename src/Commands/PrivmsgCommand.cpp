/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:12:49 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/23 13:12:56 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
