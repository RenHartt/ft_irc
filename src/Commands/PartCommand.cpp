#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>

void Command::_executePart(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2)
        throw IrcError(client->getNickname(), "PART", CLIENT_NEEDMOREPARAMS);

    std::string channel = args[1];
    if (channel[0] != '#')
        throw IrcError(client->getNickname(), "PART", CLIENT_NOSUCHCHANNEL);

    ChannelMap           channel_list = _server->getChannelsList();
    ChannelMap::iterator it = channel_list.find(channel);

    if (it == channel_list.end())
        throw IrcError(client->getNickname(), "PART", CLIENT_NOSUCHCHANNEL);

    Channel *channel_ptr = it->second;

    if (!channel_ptr->isMember(client))
        throw IrcError(client->getNickname(), "PART", CLIENT_NOTONCHANNEL);

    channel_ptr->delClient(client);

    std::string partMessage = ":" + client->getNickname() + " PART " + channel + "\r\n";
    channel_ptr->broadcastMessage(partMessage, client);

    send(client->getFd(), partMessage.c_str(), partMessage.size(), 0);
    if (channel_ptr->isMember(client) == false)
        send(client->getFd(), "You have left the channel\r\n", 27, 0);
}
