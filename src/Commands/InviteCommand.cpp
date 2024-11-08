#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>

void Command::_executeInvite(Client *client, std::vector<std::string> args)
{
	if (args.size() < 3)
	{
		std::string nickname = client->getNickname().empty() ? "*" : client->getNickname();
		throw IrcError(nickname, "INVITE", CLIENT_NEEDMOREPARAMS);
	}

	std::string nickname = args[1], channel_name = args[2];

	ChannelMap channel_list = _server->getChannelsList();
	ChannelMap::iterator it = channel_list.find(channel_name);
	Client *target = _server->getClientbyNickname(nickname);
	if (!target)
		throw IrcError(client->getNickname(), nickname, CLIENT_NOSUCHNICK);
	if (it == channel_list.end())
		throw IrcError(client->getNickname(), channel_name, CLIENT_NOSUCHCHANNEL);

	if (it->second->isMember(target))
		throw IrcError(client->getNickname(), channel_name, CLIENT_USERONCHANNEL);	
	
	
	std::string invite_message = ":" + client->getNickname() + " INVITE " + nickname + " " + channel_name + "\r\n";
	Channel *channel = it->second;
	channel->addClient(target);
	send(target->getFd(), invite_message.c_str(), invite_message.size(), 0);
}
