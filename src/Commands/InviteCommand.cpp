#include "Command.hpp"
#include "ErrTable.hpp"
#include "IrcError.hpp"
#include "Server.hpp"

void Command::_executeInvite(Client *client, std::vector<std::string> args)
{
	if (args.size() < 3 || args[1].empty() || args[2].empty())
	{
		std::string nickname = client->getNickname().empty() ? "*" : client->getNickname();
		throw IrcError(nickname, "INVITE", CLIENT_NEEDMOREPARAMS);
	}

	std::string nickname = args[1];
	std::string channel_name = args[2];

	ChannelMap channel_list = _server->getChannelsList();
	ChannelMap::iterator it = channel_list.find(channel_name);

	if (it == channel_list.end())
		throw IrcError(client->getNickname(), channel_name, CLIENT_NOSUCHCHANNEL);

	Channel *channel = it->second;
	if (channel->isMember(client))
		throw IrcError(client->getNickname(), channel_name, CLIENT_USERONCHANNEL);	

	std::string invite_message = ":" + client->getNickname() + " INVITE " + nickname + " " + channel_name + "\r\n";
	send(client->getFd(), invite_message.c_str(), invite_message.size(), 0);
}
