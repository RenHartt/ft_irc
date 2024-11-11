#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>


void sendToChannel(Client *sender, const std::string &recipient, const std::string &message, ChannelMap &channels_list)
{
    if (isValidChannelName(recipient) == false)
        throw IrcError(sender->getNickname(), recipient, CLIENT_BADCHANMASK);

    ChannelMap::iterator channel_it = channels_list.find(recipient);
    if (channel_it != channels_list.end())
    {
        Channel *channel = channel_it->second;

        if (!channel->isMember(sender))
            throw IrcError(sender->getNickname(), recipient, CLIENT_CANNOTSENDTOCHAN);
        
		std::string full_message = ":" + sender->getNickname() + "!" + sender->getUsername() + "@localhost PRIVMSG " + recipient + " :" + message + "\r\n";        
		channel->broadcastMessage(full_message, sender);

    } else
        throw IrcError(sender->getNickname(), recipient, CLIENT_NOSUCHCHANNEL);
}

void sendToClient(Client *sender, const std::string &recipient, int recipient_fd, const std::string &message)
{
    if (recipient_fd > 0)
    {
        std::string full_message = ":" + sender->getNickname() + "!" + sender->getUsername() + "@localhost PRIVMSG " + recipient + " :" + message + "\r\n";   
		send(recipient_fd, full_message.c_str(), full_message.size(), 0);
    } else
        throw IrcError(sender->getNickname(), recipient, CLIENT_NOSUCHNICK);
}

void Command::_executePrivmsg(Client *sender, std::vector<std::string> &command)
{
    if (command.size() < 2 || command[1].empty())
        throw IrcError(sender->getNickname(), CLIENT_NORECIPIENT);
    if (command.size() < 3 || command[2].empty())
        throw IrcError(sender->getNickname(), CLIENT_NOTEXTTOSEND);

    ClientMap  clients_list = _server->getClientsList();
    ChannelMap channels_list = _server->getChannelsList();

    std::vector<std::string> recipients = split(command[1], ',');
    std::string              message = command[2];

    for (std::vector<std::string>::iterator it = recipients.begin(); it != recipients.end(); it++)
    {
        std::string recipient = *it;
		int recipient_fd = _server->getClientFdByNickname(*it);
        try
        {
            if (recipient[0] == '#')
                sendToChannel(sender, recipient, message, channels_list);
            else
                sendToClient(sender, recipient, recipient_fd, message);
        } catch (const IrcError &e)
        {
            e.sendto(*sender);
        }
    }
}
