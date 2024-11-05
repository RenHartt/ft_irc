#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <iostream>
#include <sys/types.h>

void Command::_executeList(Client *client, std::vector<std::string>)
{
    std::string list;
    std::string client_nickname = client->getNickname();
    ChannelMap  channel_list = _server->getChannelsList();

    // RPL_LISTSTART
    list += "321 " + client_nickname + " Channel :Users  Name\r\n";

    for (ChannelMap::iterator it = channel_list.begin(); it != channel_list.end(); it++)
    {
        Channel    *channel = it->second;
        std::string channel_name = channel->getChannelName();
        std::string member_count = itoa(channel->getNbClient());
        std::string topic = channel->getTopic();

        list += "322 " + client_nickname + " " + channel_name + " " + member_count + " :" + topic + "\r\n";
    }

    list += "323 " + client_nickname + " :End of /LIST\r\n";

    ssize_t bytes_sent = send(client->getFd(), list.c_str(), list.size(), 0);
    if (bytes_sent != (ssize_t)list.size())
        std::cerr << "Erreur : envoi incomplet de la liste des canaux" << std::endl;
}
