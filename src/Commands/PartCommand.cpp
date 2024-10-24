/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:57:15 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 16:48:33 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include <iostream>

void Command::_executePart(Client *client, std::vector<std::string>)
{
	std::cout << "PART : " << client->getFd() << std::endl;
}

