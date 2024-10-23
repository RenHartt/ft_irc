/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:53:30 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/22 22:01:11 by bonsthie         ###   ########.fr       */
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

        int getFd() const;

      private:
        int _client_fd;

        std::string _nickname;
        std::string _username;
        std::string _realname;

        std::map<std::string, Channel *> _channels;

        std::queue<std::string> _private_message;
};
