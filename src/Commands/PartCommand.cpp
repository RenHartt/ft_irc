#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>

void Command::_executePart(Client *client, std::vector<std::string> args)
{
    std::string sender_nickname = client->getNickname();

    if (args.size() < 2)
        throw IrcError(sender_nickname, "PART", CLIENT_NEEDMOREPARAMS);

    std::string channel_name = args[1];
    Channel    *channel = _server->getChannelsList()[channel_name];
    if (!channel)
        throw IrcError(sender_nickname, "PART", CLIENT_NOSUCHCHANNEL);
    if (!channel->isMember(client))
        throw IrcError(sender_nickname, "PART", CLIENT_NOTONCHANNEL);

    channel->delGuest(client);
    channel->delClient(client);
    channel->delOperator(client);

    std::string partMessage = ":" + sender_nickname + " PART " + channel_name + "\r\n";
    channel->broadcastMessage(partMessage, NULL);
    if (channel->isMember(client) == false)
        send(client->getFd(), "You have left the channel_name\r\n", 27, 0);
}
