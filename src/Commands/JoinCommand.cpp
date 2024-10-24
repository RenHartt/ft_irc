/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:04:37 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 18:00:45 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Client.hpp"
#include <iostream>

void Command::_executeJoin(Client *client, std::vector<std::string>)
{
    std::cout << "JOIN : " << client->getFd() << std::endl;
}
