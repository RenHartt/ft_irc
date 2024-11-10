#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <sys/socket.h>

void kickTargetFromChannel(Client *client, Channel *channel, Client *target, std::string comment)
{
    std::string sender_nickname = client->getNickname();
    std::string sender_username = client->getUsername();
    std::string target_nickname = target->getNickname();
    std::string channel_name = channel->getChannelName();

    std::string message = ":" + sender_nickname + "!" + sender_username + "@localhost KICK " +
                          channel_name + " " + target_nickname + " :" + comment + "\r\n";
    channel->broadcastMessage(message, NULL);

    channel->delGuest(target);
    channel->delClient(target);
    channel->delOperator(target);
}

void Command::_executeKick(Client *client, std::vector<std::string> args)
{
    std::string sender_nickname = client->getNickname();

    if (args.size() < 3)
        throw IrcError(sender_nickname, "KICK", CLIENT_NEEDMOREPARAMS);

    std::string channel_name = args[1];
    std::string target_nickname = args[2];
    std::string comment = (args.size() > 3) ? args[3] : "No reason given";

    Channel *channel = _server->getChannelByChannelname(channel_name);
    if (!channel)
        throw IrcError(sender_nickname, channel_name, CLIENT_NOSUCHCHANNEL);
    if (!channel->isMember(client))
        throw IrcError(sender_nickname, channel_name, CLIENT_NOTONCHANNEL);
    if (!channel->isOperator(client))
        throw IrcError(sender_nickname, channel_name, CLIENT_CHANOPRIVSNEEDED);

    Client *target = _server->getClientByNickname(target_nickname);
    if (!target)
        throw IrcError(sender_nickname, target_nickname, CLIENT_NOSUCHNICK);
    if (!channel->isMember(target))
        throw IrcError(sender_nickname, target_nickname, CLIENT_USERNOTINCHANNEL);

    kickTargetFromChannel(client, channel, target, comment);
}
