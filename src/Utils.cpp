/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:17:48 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 17:16:17 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string> Server::splitCommand(const char *buffer)
{
    std::vector<std::string> splited;
    std::string              command(buffer);
    std::size_t              colonPos = command.find(':');
    std::string              trailing;

    if (colonPos != std::string::npos)
    {
        trailing = command.substr(colonPos + 1);
        command = command.substr(0, colonPos);
    }

    std::stringstream ss(command);
    std::string       token;

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

std::string itoa(int value)
{
    std::stringstream ss;
    ss << value;
    return (ss.str());
}
