/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:12:49 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 17:01:00 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"

void Command::_executePrivmsg(Client *sender, std::vector<std::string> command)
{
    if (command.size() < 3)
    {
        std::string error_message = "Usage : PRIVMSG <recipient> :<message>\n";
        send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
        return;
    }

    std::string recipient = command[1];
    std::string message = command[2];

	std::map<std::string, Client *> clients_list = _server->getClientsList();
    if (clients_list.find(recipient) != clients_list.end())
    {
        Client     *recipient_client = clients_list[recipient];
        std::string full_message = sender->getNickname() + " : " + message;
        send(recipient_client->getFd(), full_message.c_str(),
             full_message.size(), 0);
    } else
    {
        std::string error_message = "Client not found\n";
        send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
        return;
    }
}
