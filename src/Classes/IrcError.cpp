/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babonnet <babonnet@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by babonnet          #+#    #+#             */
/*   Updated: 2024/11/11 21:37:10 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <Client.hpp>
#include <ErrTable.hpp>
#include <IrcError.hpp>

/* bad because of cpp98 */
const std::string IrcError::_map_error[] = {
    "",                      // NONE
    "Server",                // SERVER
    "Server initialisation", // SERVER_INIT
    "Client",                // CLIENT

    "client nonicknamegiven",   // CLIENT_NONICKNAMEGIVEN
    "client alreadyregistered", // CLIENT_ALREADYREGISTERED
    "client passwdmismatch",    // CLIENT_PASSWDMISMATCH
    "client notregistered",     // CLIENT_NOTREGISTERED
    "client norecipient",       // CLIENT_NORECIPIENT
    "client notexttosend",      // CLIENT_NOTEXTTOSEND
    "client needmoreparams",    // CLIENT_NEEDMOREPARAMS
    "client notonchannel",      // CLIENT_NOTONCHANNEL
    "client channelisfull",     // CLIENT_CHANNELISFULL
    "client badchannelkey",     // CLIENT_BADCHANNELKEY
    "client inviteonlychan",    // CLIENT_INVITEONLYCHAN
    "client nosuchchannel",     // CLIENT_NOSUCHCHANNEL
    "client chanoprivsneeded",  // CLIENT_CHANOPRIVSNEEDED
    "client badchanmask",       // CLIENT_BADCHANMASK
    "client cannotsendtochan",  // CLIENT_CANNOTSENDTOCHAN
	"client nicknameinuse",     // CLIENT_NICKNAMEINUSE
	"client erroneusnickname",  // CLIENT_ERRONEUSNICKNAME
    "client unknowncommand",    // CLIENT_UNKNOWNCOMMAND
    "client nosuchnick",        // CLIENT_NOSUCHNICK
    "client usernotinchannel",  // CLIENT_USERNOTINCHANNEL
    "client useronchannel",     // CLIENT_USERONCHANNEL
};

/* constructor */

IrcError::IrcError(const std::string &msg, e_IrcErrorType type) : _type(type)
{
    switch (type)
    {
    case SERVER:
        _msg = msg;
        break;
    case CLIENT_NONICKNAMEGIVEN:
        _msg = ERR_NONICKNAMEGIVEN(msg);
        break;
    case CLIENT_ALREADYREGISTERED:
        _msg = ERR_ALREADYREGISTERED(msg);
        break;
    case CLIENT_PASSWDMISMATCH:
        _msg = ERR_PASSWDMISMATCH(msg);
        break;
    case CLIENT_NOTREGISTERED:
        _msg = ERR_NOTREGISTERED(msg);
        break;
    case CLIENT_NORECIPIENT:
        _msg = ERR_NORECIPIENT(msg);
        break;
    case CLIENT_NOTEXTTOSEND:
        _msg = ERR_NOTEXTTOSEND(msg);
        break;
    default:
        _msg = msg;
    }
}

IrcError::IrcError(const std::string &msg, const std::string &msg2, e_IrcErrorType type)
    : _type(type)
{
    switch (type)
    {
    case CLIENT_NEEDMOREPARAMS:
        _msg = ERR_NEEDMOREPARAMS(msg, msg2);
        break;
    case CLIENT_NOTONCHANNEL:
        _msg = ERR_NOTONCHANNEL(msg, msg2);
        break;
    case CLIENT_CHANNELISFULL:
        _msg = ERR_CHANNELISFULL(msg, msg2);
        break;
    case CLIENT_BADCHANNELKEY:
        _msg = ERR_BADCHANNELKEY(msg, msg2);
        break;
    case CLIENT_INVITEONLYCHAN:
        _msg = ERR_INVITEONLYCHAN(msg, msg2);
        break;
    case CLIENT_NOSUCHCHANNEL:
        _msg = ERR_NOSUCHCHANNEL(msg, msg2);
        break;
    case CLIENT_CHANOPRIVSNEEDED:
        _msg = ERR_CHANOPRIVSNEEDED(msg, msg2);
        break;
    case CLIENT_BADCHANMASK:
        _msg = ERR_BADCHANMASK(msg, msg2);
        break;
    case CLIENT_CANNOTSENDTOCHAN:
        _msg = ERR_CANNOTSENDTOCHAN(msg, msg2);
        break;
    case CLIENT_NICKNAMEINUSE:
        _msg = ERR_NICKNAMEINUSE(msg, msg2);
        break;
    case CLIENT_NOSUCHNICK:
        _msg = ERR_NOSUCHNICK(msg, msg2);
        break;
    case CLIENT_ERRONEUSNICKNAME:
        _msg = ERR_ERRONEUSNICKNAME(msg, msg2);
        break;
    case CLIENT_UNKNOWNCOMMAND:
        _msg = ERR_UNKNOWNCOMMAND(msg, msg2);
        break;
    default:
        _msg = msg + " " + msg2;
    }
}

IrcError::IrcError(const std::string &msg, const std::string &msg2, const std::string &msg3,
                   e_IrcErrorType type)
    : _type(type)
{
    switch (type)
    {
    case CLIENT_USERNOTINCHANNEL:
        _msg = ERR_USERNOTINCHANNEL(msg, msg2, msg3);
        break;
    case CLIENT_USERONCHANNEL:
        _msg = ERR_USERONCHANNEL(msg, msg2, msg3);
        break;
    default:
        _msg = msg + " " + msg2 + " " + msg3;
    }
}

/* getter */

e_IrcErrorType IrcError::getIrcErrorType(void) const { return _type; }
std::string    IrcError::getMsg(void) const { return _msg; }

/* function */

void IrcError::log(void) const
{
    std::cerr << _map_error[_type] << " Error: " << _msg << std::endl;
}

ssize_t IrcError::sendto(const Client &sender) const
{
    return (send(sender.getFd(), _msg.c_str(), _msg.length(), 0));
}
