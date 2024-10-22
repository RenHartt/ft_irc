/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:04:37 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 17:37:46 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
	: _IP("127.0.0.1"), \
	_nickname("guest"), \
	_username("guest"), \
	_realname("Guest User"), \
	_server("localhost")
{
}

Client::Client(const std::string& ip, \
			const std::string& nickname, \
			const std::string& username, \
			const std::string& realname)
	: _IP(ip), _nickname(nickname), _username(username), _realname(realname), _server("localhost")
{
}

Client::~Client()
{
}

const std::string& Client::getIP() const { return _IP; }
const std::string& Client::getNickname() const { return _nickname; }
const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getRealname() const { return _realname; }

void Client::setIP(const std::string& ip) { _IP = ip; }
void Client::setNickname(const std::string& nickname) { _nickname = nickname; }
void Client::setUsername(const std::string& username) { _username = username; }
void Client::setRealname(const std::string& realname) { _realname = realname; }

void Client::setServer(const std::string& server) { _server = server; }
const std::string& Client::getServer() const { return _server; }

