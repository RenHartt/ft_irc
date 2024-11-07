#include <Utils.hpp>
#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <sys/socket.h>
#include <iostream>

Channel *getChannel(Client *client, ChannelMap channels_list, const std::string &channel_name)
{
	std::string client_nickname = client->getNickname();

    ChannelMap::iterator channel_it = channels_list.find(channel_name);
    if (channel_it == channels_list.end())
        throw IrcError(client_nickname, channel_name, CLIENT_NOSUCHCHANNEL);

    Channel *channel = channel_it->second;
    if (!channel->isMember(client))
        throw IrcError(client_nickname, channel_name, CLIENT_NOTONCHANNEL);

    return channel;
}

void kickTargetFromChannel(Client *client, Channel *channel, Client *target, const std::string &comment)
{
	if (!target)
		throw IrcError(client->getNickname(), "KICK", CLIENT_NOSUCHNICK);

	std::string client_nickname = client->getNickname();
	std::string target_nickname = target->getNickname();
	std::string channel_name = channel->getChannelName();

    if (!channel->isMember(target))
        throw IrcError(client_nickname, target_nickname, CLIENT_USERNOTINCHANNEL);

    channel->delClient(target);

    std::string kick_message = ":" + client_nickname + " KICK " + channel_name + " " + target_nickname + " :" + comment + "\r\n";
    channel->broadcastMessage(kick_message, client);

    std::string notify_message = "You have been kicked from " + channel_name + " by " + client_nickname + " : " + comment + "\r\n";
    send(target->getFd(), notify_message.c_str(), notify_message.size(), 0);
}

void Command::_executeKick(Client *client, std::vector<std::string> args)
{
    if (args.size() < 3)
        throw IrcError(client->getNickname(), "KICK", CLIENT_NEEDMOREPARAMS);

    std::vector<std::string> channels = split(args[1], ',');
    std::vector<std::string> targets = split(args[2], ',');
    std::string comment = (args.size() > 3) ? args[3] : "No reason given";

	ChannelMap channels_list = _server->getChannelsList();
	ClientMap clients_list = _server->getClientsList();

	for (std::vector<std::string>::iterator channel_it = channels.begin(); channel_it != channels.end(); channel_it++)
	{
		for (std::vector<std::string>::iterator client_it = targets.begin(); client_it != targets.end(); client_it++)
		{
			try
			{
				Client *target = _server->getClientbyNickname(*client_it); 
				Channel *channel = getChannel(client, channels_list, *channel_it);
				std::cout << target << std::endl;

				kickTargetFromChannel(client, channel, target, comment);
			}
			catch (const IrcError& e)
			{
				e.sendto(*client);
			}
		}
	}
}
