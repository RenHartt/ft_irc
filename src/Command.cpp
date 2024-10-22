/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:16:51 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 20:20:57 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::executeUser(Client *client, std::vector<std::string>)
{
	std::cout << "USER : " << client->getFd() << std::endl;
}

void Server::executeNick(Client *client, std::vector<std::string>)
{
	std::cout << "NICK : " << client->getFd() << std::endl;
}

void Server::executePrivmsg(Client *client, std::vector<std::string>)
{
	std::cout << "PRIVMSG : " << client->getFd() << std::endl;
}

void Server::executeQuit(Client *client, std::vector<std::string>)
{
	std::cout << "Destruction -> " << client << std::endl;
	close(client->getFd());
	for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end();)
	{
		it->fd == client->getFd() ? it = _poll_fds.erase(it) : it++;
	}
	_clients_list.erase(client->getFd());
	delete client;

	std::cout << "Client deconnecte" << std::endl;
}
