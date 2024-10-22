/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:16:51 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 13:57:32 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::executeUser(Client &client)
{
	std::cout << "USER : " << client.getFd() << std::endl;
}

void Server::executeNick(Client &client)
{
	std::cout << "NICK : " << client.getFd() << std::endl;
}

void Server::executePrivmsg(Client &client)
{
	std::cout << "PRIVMSG : " << client.getFd() << std::endl;
}

void Server::executeQuit(Client &client)
{
	close(client.getFd());
	for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end();)
	{
		it->fd == client.getFd() ? it = _poll_fds.erase(it) : it++;
	}
	_clients.erase(client.getFd());

	std::cout << "Client deconnecte" << std::endl;
}

