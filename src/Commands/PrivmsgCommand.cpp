#include "Utils.hpp"
#include <Command.hpp>
#include <Server.hpp>


void Command::_executePrivmsg(Client *sender, std::vector<std::string> command)
{
    if (command.size() < 2 || command[1].empty())
    {
        std::string error_message = ERR_NORECIPIENT(sender->getNickname());
        send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
        return;
    }

    if (command.size() < 3 || command[2].empty())
    {
        std::string error_message = ERR_NOTEXTTOSEND(sender->getNickname());
        send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
        return;
    }

    std::map<int, Client *> clients_list = _server->getClientsList();
    std::string recipient = command[1];
    std::string message = command[2];
    int recipient_fd = getFdByNickname(recipient, clients_list);
    if (recipient_fd > 0)
    {
        std::string full_message = ":" + sender->getNickname() + " PRIVMSG " + recipient + " :" + message + "\r\n";
        send(recipient_fd, full_message.c_str(), full_message.size(), 0);
    }
    else 
    {
        std::string error_message = ERR_NOSUCHNICK(sender->getNickname(), recipient);
        send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
    }
}

