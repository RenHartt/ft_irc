/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:12:27 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/23 20:39:07 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::executeUser(Client *sender, std::vector<std::string>)
{
    std::string message = "NickName -> " + sender->getNickname() + "\n";
    send(sender->getFd(), message.c_str(), message.size(), 0);
    message = "fd -> " + itoa(sender->getFd()) + "\n";
    send(sender->getFd(), message.c_str(), message.size(), 0);
}
