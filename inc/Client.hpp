/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:53:30 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 18:33:14 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <queue>
#include <string>

#include "Channel.hpp"

class Channel;

class Client
{
  public:
    Client(int fd);

    int          getFd();
    std::string &getNickname();
    std::string &getUsername();
    std::string &getRealname();

    void setNickname(const std::string &nickname);
    void setUsername(std::string &username);
    void setRealname(std::string &realname);

  private:
    int         _client_fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;

    std::map<std::string, Channel *> _channels;

    std::queue<std::string> _private_message;
};
