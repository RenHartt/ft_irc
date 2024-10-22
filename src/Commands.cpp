/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:16:51 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 18:01:46 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::executeUser(int client_fd)
{
	std::cout << "USER : " << client_fd << std::endl;
}

void Server::executeNick(int client_fd)
{
	std::cout << "NICK : " << client_fd << std::endl;
}

void Server::executePrivmsg(int client_fd)
{
	std::cout << "PRIVMSG : " << client_fd << std::endl;
}

void Server::executeQuit(int client_fd)
{
	close(client_fd);
	for (std::vector<pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end();)
	{
		it->fd == client_fd ? it = _poll_fds.erase(it) : it++;
	}
	_clients.erase(client_fd);

	std::cout << "Client deconnecte" << std::endl;
}

