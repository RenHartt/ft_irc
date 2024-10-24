/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:13:06 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 17:01:19 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Client.hpp"
#include <iostream>

void Command::_executeNick(Client *client, std::vector<std::string>)
{
    std::cout << "NICK : " << client->getFd() << std::endl;
}
