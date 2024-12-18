#include <IrcError.hpp>
#include <Server.hpp>

void Command::_executePart(Client *client, std::vector<std::string> &args)
{
    std::string sender_nickname = client->getNickname();
    std::string sender_username = client->getUsername();

    if (args.size() < 2)
        throw IrcError(sender_nickname, "PART", CLIENT_NEEDMOREPARAMS);

    std::vector<std::string> channels = split(args[1], ',');

    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        try
        {
            std::string channel_name = *it;
            Channel    *channel = _server->getChannelsList()[channel_name];
            if (!channel)
                throw IrcError(sender_nickname, "PART", CLIENT_NOSUCHCHANNEL);
            if (!channel->isMember(client))
                throw IrcError(sender_nickname, "PART", CLIENT_NOTONCHANNEL);

            std::string message = ":" + sender_nickname + "!" + sender_username + "@localhost PART " + channel_name + "\r\n";
            channel->broadcastMessage(message, NULL);

            channel->delGuest(client);
            channel->delClient(client);
            channel->delOperator(client);
        } catch (const IrcError &e)
        {
            e.sendto(*client);
        }
    }
}
