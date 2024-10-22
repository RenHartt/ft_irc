/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:53:30 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 17:38:18 by babonnet         ###   ########.fr       */
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
		Client(const std::string& ip, const std::string& nickname, const std::string& username, const std::string& realname);
		~Client();
		const std::string& getIP() const;
		const std::string& getNickname() const;
		const std::string& getUsername() const;
		const std::string& getRealname() const;

		void setIP(const std::string& ip);
		void setNickname(const std::string& nickname);
		void setUsername(const std::string& username);
		void setRealname(const std::string& realname);
		
		void setServer(const std::string& server); 
		const std::string& getServer() const; 

	private:
		std::string				_IP;
		std::string				_nickname; 
		std::string				_username; 
		std::string				_realname;
		std::set<std::string>	_channels;
		std::queue<std::string> _private_message;
		std::string				_server; 
};

