#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>

void Command::_executeInvite(Client *client, std::vector<std::string> args)
{
    if (args.size() < 3)
        throw IrcError(client->getNickname(), "INVITE", CLIENT_NEEDMOREPARAMS);

    std::string          nickname = args[1], channel_name = args[2];
    ChannelMap           channel_list = _server->getChannelsList();
    ChannelMap::iterator it = channel_list.find(channel_name);
    Client              *target = _server->getClientbyNickname(nickname);

    if (!target)
        throw IrcError(client->getNickname(), nickname, CLIENT_NOSUCHNICK);
    if (it == channel_list.end())
        throw IrcError(client->getNickname(), channel_name, CLIENT_NOSUCHCHANNEL);
    if (!it->second->isMember(client))
        throw IrcError(client->getNickname(), channel_name, CLIENT_NOTONCHANNEL);
    if (it->second->isMember(target))
        throw IrcError(client->getNickname(), channel_name, CLIENT_USERONCHANNEL);

    it->second->addGuest(target);

    std::string invite_message = ":" + client->getNickname() + " INVITE " + target->getNickname() + " :" + channel_name + "\r\n";
    send(target->getFd(), invite_message.c_str(), invite_message.size(), 0);

    std::string confirm_message = ":localhost 341 " + client->getNickname() + " " + target->getNickname() + " :" + channel_name + "\r\n";
    send(client->getFd(), confirm_message.c_str(), confirm_message.size(), 0);
}
