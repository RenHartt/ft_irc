#include "Server.hpp"
#include "IrcError.hpp"
#include "ErrTable.hpp"
#include "Channel.hpp"


void Command::_executeTopic(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2) {
        std::string error = ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC");
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }

    std::string channel_name = args[1];
    ChannelMap channel_list = _server->getChannelsList();

    ChannelMap::iterator it = channel_list.find(channel_name);
    if (it == channel_list.end()) {
        std::string error = ERR_NOSUCHCHANNEL(client->getNickname(), channel_name);
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }

    Channel *channel = it->second;

    if (!channel->isMember(client)) {
        std::string error = ERR_NOTONCHANNEL(client->getNickname(), channel_name);
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }

    if (args.size() == 2) {
        std::string topic = channel->getTopic();
        if (topic.empty()) {
            std::string response = "331 " + client->getNickname() + " " + channel_name + " :No topic is set\r\n";
            send(client->getFd(), response.c_str(), response.size(), 0);
        } else {
            std::string response = "332 " + client->getNickname() + " " + channel_name + " :" + topic + "\r\n";
            send(client->getFd(), response.c_str(), response.size(), 0);
        }
        return;
    }

    std::string new_topic = args[2];
    if (!client->isOperator()) {  
        std::string error = ERR_CHANOPRIVSNEEDED(client->getNickname(), channel_name);
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }

    channel->setTopic(new_topic);
    std::string confirmation = ":" + client->getNickname() + " TOPIC " + channel_name + " :" + new_topic + "\r\n";
    channel->broadcastMessage(confirmation, client);  // Notifie tous les membres du channel
}

