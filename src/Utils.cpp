/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:17:48 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 17:46:28 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::isCommand(const std::string &command)
{
	return (command == "USER"    ||
			command == "NICK"    ||
			command == "PRIVMSG" ||
			command == "QUIT");
}

std::vector<std::string> Server::splitCommand(const char *buffer)
{
    std::vector<std::string> splited;
    std::string command(buffer);
    std::size_t colonPos = command.find(':');
    std::string trailing;

    if (colonPos != std::string::npos)
    {
        trailing = command.substr(colonPos);
        command = command.substr(0, colonPos);
    }

    std::stringstream ss(command);
    std::string token;
    
	while (ss >> token)
	{
        splited.push_back(token);
	}
	if (!trailing.empty())
	{
		splited.push_back(trailing);
	}

	return (splited);
}

