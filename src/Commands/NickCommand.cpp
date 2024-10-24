/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:13:06 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 18:36:14 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"

void Server::updateNickname(int client_fd, const std::string &new_nickname)
{
    Client *client = _clients_list[itoa(client_fd)];

    for (std::map<std::string, Client *>::iterator it = _clients_list.begin(); it != _clients_list.end(); ++it)
    {
        if (it->second == client)
        {
            _clients_list.erase(it);
            break;
        }
    }

    client->setNickname(new_nickname);
    _clients_list[new_nickname] = client;
    std::cout << "Client with FD " << client_fd << " has taken nickname: " << new_nickname << std::endl;
}

void Command::_executeNick(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        std::string message = "Please enter a nickname\n";
        send(client->getFd(), message.c_str(), message.size(), 0);
        std::cout << "Please enter a nickname" << std::endl;
    }
    else
    {
        std::string newNickname = args[1]; 
        client->setNickname(newNickname); 

        std::string message = "Your nickname is " + client->getNickname() + "\n";
		_server->updateNickname(client->getFd(), newNickname);
        send(client->getFd(), message.c_str(), message.size(), 0);
    }
}
