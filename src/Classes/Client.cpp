/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:04:37 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 18:33:26 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

Client::Client(int client_fd)
    : _client_fd(client_fd),
      _nickname(itoa(client_fd))
{}

int Client::getFd() { return (this->_client_fd); }

std::string &Client::getNickname() { return _nickname; }
std::string &Client::getUsername() { return _username; }
std::string &Client::getRealname() { return _realname; }

void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
void Client::setUsername(std::string &username) { _username = username; }
void Client::setRealname(std::string &realname) { _realname = realname; }
