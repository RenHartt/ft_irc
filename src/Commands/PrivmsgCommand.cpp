/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:12:49 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 18:28:36 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include <cctype>  

#include <cctype>  // Pour ::isdigit
#include <sstream> // Pour std::stringstream

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
    
    bool is_fd = true;
    for (size_t i = 0; i < recipient.size(); ++i)
    {
        if (!std::isdigit(recipient[i]))
        {
            is_fd = false;
            break;
        }
    }

    std::map<std::string, Client *> clients_list = _server->getClientsList();

    if (is_fd)
    {
        std::stringstream ss(recipient);
        int recipient_fd;
        ss >> recipient_fd;

        for (std::map<std::string, Client *>::iterator it = clients_list.begin(); it != clients_list.end(); ++it)
        {
            if (it->second->getFd() == recipient_fd)
            {
                std::string full_message = sender->getNickname() + " : " + message;
                send(recipient_fd, full_message.c_str(), full_message.size(), 0);
                return;
            }
        }
        std::string error_message = "Client with FD not found\n";
        send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
        return;
    }
    else
    {
        if (clients_list.find(recipient) != clients_list.end())
        {
            Client *recipient_client = clients_list[recipient];
            std::string full_message = sender->getNickname() + " : " + message;
            send(recipient_client->getFd(), full_message.c_str(), full_message.size(), 0);
        }
        else
        {
            std::string error_message = "Client not found\n";
            send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
        }
    }
}
