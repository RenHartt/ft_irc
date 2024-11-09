#include <Utils.hpp>
#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <sys/socket.h>

Channel *getChannel(Client *client, ChannelMap channels_list, std::string channel_name)
{
    std::string client_nickname = client->getNickname();
    ChannelMap::iterator channel_it = channels_list.find(channel_name);
    Channel *channel = channel_it->second;

    if (channel_it == channels_list.end())
        throw IrcError(client_nickname, channel_name, CLIENT_NOSUCHCHANNEL);
    if (!channel->isMember(client))
        throw IrcError(client_nickname, channel_name, CLIENT_NOTONCHANNEL);
    if (!channel->isOperator(client))
        throw IrcError(client_nickname, channel_name, CLIENT_CHANOPRIVSNEEDED);

    return channel;
}

void kickTargetFromChannel(Client *client, Channel *channel, Client *target, std::string comment)
{
    std::string client_nickname = client->getNickname();
	std::string client_username = client->getUsername();
    std::string target_nickname = target->getNickname();
    std::string channel_name = channel->getChannelName();

    if (!channel->isMember(target))
        throw IrcError(client_nickname, target_nickname, CLIENT_USERNOTINCHANNEL);

    std::string message = ":" + client_nickname + "!" + client_username + "@localhost KICK " + channel_name + " " + target_nickname + " :" + comment + "\r\n";
    channel->broadcastMessage(message, NULL);

	channel->delGuest(target);
    channel->delClient(target);
    channel->delOperator(target);
}

void Command::_executeKick(Client *client, std::vector<std::string> args)
{
    if (args.size() < 3)
        throw IrcError(client->getNickname(), "KICK", CLIENT_NEEDMOREPARAMS);

    std::string channel_name = args[1];
    std::string target_nickname = args[2];
    std::string comment = (args.size() > 3) ? args[3] : "No reason given";

    ChannelMap channels_list = _server->getChannelsList();
    Client *target = _server->getClientbyNickname(target_nickname);
    
    if (!target)
        throw IrcError(client->getNickname(), target_nickname, CLIENT_NOSUCHNICK);

    Channel *channel = getChannel(client, channels_list, channel_name);
    kickTargetFromChannel(client, channel, target, comment);
}
