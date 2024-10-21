/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:53:30 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/21 22:19:21 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <queue>
#include <set>

class Client
{
	public:
		Client();
	
	private:
		/* int						_fd;  */
		/* int						_port; */
		std::string				_IP; 
	
		std::string				_nickname; 
		std::string				_username; 
		std::string				_realname; 
		
		std::set<std::string>	_channels;
	
		std::queue<std::string> _private_message;
};


