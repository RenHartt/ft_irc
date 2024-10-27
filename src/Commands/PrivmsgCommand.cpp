#include <Command.hpp>
#include <ErrTable.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <cstring>

void Command::sendToChannel(Client *sender, const std::string &recipient,
                            const std::string &message, ChannelMap &channels_list)
{
    if (!isValidChannelName(recipient))
        throw IrcError(sender->getNickname(), recipient, CLIENT_BADCHANMASK);

    ChannelMap::iterator channel_it = channels_list.find(recipient);
    if (channel_it != channels_list.end())
    {
        Channel *channel = channel_it->second;

<<<<<<< HEAD
        if (!channel->isMember(sender))
            throw IrcError(sender->getNickname(), recipient, CLIENT_CANNOTSENDTOCHAN);

        std::string full_message = sender->getNickname() + " : " + message;
        channel->broadcastMessage(full_message, sender);
    } else
        throw IrcError(sender->getNickname(), recipient, CLIENT_NOSUCHCHANNEL);
=======
		if (!channel->isMember(sender))
		{
			std::string error_message = ERR_CANNOTSENDTOCHAN(sender->getNickname(), recipient);
			send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
		} else
		{
			std::string full_message = sender->getNickname() + " : " + message;
			channel->broadcastMessage(full_message, sender);
		}
	} else
	{
		std::string error_message = ERR_NOSUCHCHANNEL(sender->getNickname(), recipient);
		send(sender->getFd(), error_message.c_str(), error_message.size(), 0);
	}
>>>>>>> refs/remotes/origin/main
}

void Command::sendToClient(Client *sender, const std::string &recipient, const std::string &message,
                           ClientMap &clients_list)
{
    int recipient_fd = getFdByNickname(recipient, clients_list);
    if (recipient_fd > 0)
    {
        std::string full_message = sender->getNickname() + " : " + message;
        send(recipient_fd, full_message.c_str(), full_message.size(), 0);
    } else
        throw IrcError(sender->getNickname(), recipient, CLIENT_NOSUCHNICK);
}

void Command::routePrivmsg(Client *sender, std::vector<std::string> command)
{
    ClientMap  clients_list = _server->getClientsList();
    ChannelMap channels_list = _server->getChannelsList();

    std::vector<std::string> recipients = split(command[1], ',');
    std::string              message = command[2];

    for (std::vector<std::string>::iterator it = recipients.begin(); it != recipients.end(); ++it)
    {
        std::string recipient = *it;
        try
        {
            if (recipient[0] == '#' || recipient[0] == '&')
                sendToChannel(sender, recipient, message, channels_list);
            else
                sendToClient(sender, recipient, message, clients_list);
        } catch (const IrcError &e)
        {
            e.sendto(*sender);
        }
    }
}

void Command::_executePrivmsg(Client *sender, std::vector<std::string> command)
{
    try
    {
        if (command.size() < 2 || command[1].empty())
            throw IrcError(sender->getNickname(), CLIENT_NORECIPIENT);

        if (command.size() < 3 || command[2].empty())
            throw IrcError(sender->getNickname(), CLIENT_NOTEXTTOSEND);

        routePrivmsg(sender, command);
    } catch (const IrcError &e)
    {
        e.sendto(*sender);
    }
}
