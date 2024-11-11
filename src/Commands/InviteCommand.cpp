#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>

void Command::_executeInvite(Client *client, std::vector<std::string> &args)
{
    std::string sender_nickname = client->getNickname();

    if (args.size() < 3)
        throw IrcError(sender_nickname, "INVITE", CLIENT_NEEDMOREPARAMS);

    std::string target_nickname = args[1];
    std::string channel_name = args[2];

    Client *target = _server->getClientByNickname(target_nickname);
    if (!target)
        throw IrcError(sender_nickname, target_nickname, CLIENT_NOSUCHNICK);

    Channel *channel = _server->getChannelsList()[channel_name];
    if (!channel)
        throw IrcError(sender_nickname, channel_name, CLIENT_NOSUCHCHANNEL);
    if (!channel->isMember(client))
        throw IrcError(sender_nickname, channel_name, CLIENT_NOTONCHANNEL);
    if (!channel->isOperator(client))
        throw IrcError(sender_nickname, channel_name, CLIENT_CHANOPRIVSNEEDED);
    if (channel->isMember(target))
        throw IrcError(sender_nickname, channel_name, CLIENT_USERONCHANNEL);

    channel->addGuest(target);

    std::string message = ":" + sender_nickname + " INVITE " + target_nickname + " :" + channel_name + "\r\n";
    send(target->getFd(), message.c_str(), message.size(), 0);

    message = ":localhost 341 " + sender_nickname + " " + target_nickname + " :" + channel_name + "\r\n";
    send(client->getFd(), message.c_str(), message.size(), 0);
}
