#include "Utils.hpp"
#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <sys/socket.h>

void Command::_executeKick(Client *client, std::vector<std::string> args)
{
    if (args.size() < 3)
        throw IrcError(client->getNickname(), "KICK", CLIENT_NEEDMOREPARAMS);

    std::vector<std::string> channels = split(args[1], ',');
    std::vector<std::string> targets = split(args[2], ',');
    std::string comment = (args.size() > 3) ? args[3] : "No reason given";

    ChannelMap channels_list = _server->getChannelsList();
    ClientMap clients_list = _server->getClientsList();

    for (std::vector<std::string>::iterator ch_it = channels.begin(); ch_it != channels.end(); ch_it++)
    {
        std::string channel_name = *ch_it;
        ChannelMap::iterator channel_it = channels_list.find(channel_name);

        try
        {
            if (channel_it == channels_list.end())
                throw IrcError(client->getNickname(), channel_name, CLIENT_NOSUCHCHANNEL);

            Channel* channel = channel_it->second;

            if (!channel->isMember(client))
                throw IrcError(client->getNickname(), channel_name, CLIENT_NOTONCHANNEL);

            for (std::vector<std::string>::iterator cli_it = targets.begin(); cli_it != targets.end(); cli_it++)
            {
                std::string target_nick = *cli_it;
                ClientMap::iterator target_it = clients_list.find(getFdByNickname(target_nick, clients_list));

                try
                {
                    if (target_it == clients_list.end() || !channel->isMember(target_it->second))
                        throw IrcError(client->getNickname(), target_nick, CLIENT_USERNOTINCHANNEL);

                    Client* target_client = target_it->second;

                    channel->delClient(target_client);

                    std::string kick_message = ":" + client->getNickname() + " KICK " + channel_name + " " + target_nick + " :" + comment + "\r\n";
                    channel->broadcastMessage(kick_message, client);

                    std::string notify_message = "You have been kicked from " + channel_name + " by " + client->getNickname() + " : " + comment + "\r\n";
                    send(target_client->getFd(), notify_message.c_str(), notify_message.size(), 0);
                }
                catch (const IrcError& e)
                {
                    e.sendto(*client);
                }
            }
        }
        catch (const IrcError& e)
        {
            e.sendto(*client);
        }
    }
}
