/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:16:51 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/23 20:38:49 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::executeQuit(Client *client, std::vector<std::string>)
{
    close(client->getFd());
    for (std::vector<pollfd>::iterator it = _poll_fds.begin();
         it != _poll_fds.end();)
    {
        it->fd == client->getFd() ? it = _poll_fds.erase(it) : it++;
    }
    _clients_list.erase(itoa(client->getFd()));
    delete client;

    std::cout << "Client deconnecte" << std::endl;
}
