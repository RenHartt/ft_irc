/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:11:53 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/21 19:51:27 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <iostream>

Command::~Command() {}

void PrivMsg::execute()
{
	std::cout << "PRIVMSG" << std::endl;		
}

void User::execute()
{
	std::cout << "USER" << std::endl;		
}

void Nick::execute()
{
	std::cout << "NICK" << std::endl;		
}

void Quit::execute()
{
	std::cout << "QUIT" << std::endl;		
}
