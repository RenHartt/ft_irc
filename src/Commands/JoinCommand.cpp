#include "Channel.hpp"
#include <Client.hpp>
#include <Command.hpp>
#include <ErrTable.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <sys/socket.h>

std::string getListOfClients(Channel *channel)
{
    std::string clients_list;
	ClientMap clients = channel->getClientsMap();

    for (ClientMap::iterator it = clients.begin(); it != clients.end(); it++)
    {
        channel->isOperator(it->second) ? clients_list += "@" + it->second->getNickname()
                                        : clients_list += it->second->getNickname();

        if (it != clients.end())
            clients_list += " ";
    }

    return clients_list;
}

ChannelPasswordList initRequestList(std::vector<std::string> args)
{
    std::vector<std::string> channels_list;
	std::vector<std::string> password_list;
    ChannelPasswordList      request_list;

    if (args.size() > 1)
        channels_list = split(args[1], ',');
    if (args.size() > 2)
        password_list = split(args[2], ',');

    for (std::size_t i = 0; i < channels_list.size(); i++)
    {
        std::string password = (i < password_list.size()) ? password_list[i] : "";
        request_list.push_back(std::make_pair(channels_list[i], password));
    }

    return request_list;
}

void createChannel(Client *client, const std::string &channel_name, const std::string &password, Server *server)
{
    std::string sender_nickname = client->getNickname();
    std::string sender_username = client->getUsername();

    if (isValidChannelName(channel_name) == false)
        throw IrcError(sender_nickname, channel_name, CLIENT_BADCHANMASK);

    Channel *newChannel;
    password.empty() ? newChannel = new Channel(channel_name)
                     : newChannel = new Channel(channel_name, password);

    server->addChannel(channel_name, newChannel);
    newChannel->addClient(client);
    newChannel->addOperator(client);

    std::string message;
    message = ":" + sender_nickname + "!" + sender_username + "@localhost" + " JOIN " + channel_name + "\r\n";
    newChannel->broadcastMessage(message, NULL);

    message = ":localhost 353 " + sender_nickname + " = " + channel_name + " :@" + sender_nickname + "\r\n";
    message += ":localhost 366 " + sender_nickname + " " + channel_name + " :End of /NAMES list\r\n";
    send(client->getFd(), message.c_str(), message.length(), 0);
}

void joinChannel(Client *client, Channel *channel, const std::string &password)
{
    std::string sender_nickname = client->getNickname();
    std::string channel_name = channel->getChannelName();
	ClientMap clients = channel->getClientsMap();
	ChannelSettings channel_settings = channel->getChannelSettings();

    if (channel_settings.i_inviteOnly && !channel->isGuest(client))
        throw IrcError(sender_nickname, channel_name, CLIENT_INVITEONLYCHAN);
    if (channel_settings.k_enableKey && channel->getPassword() != password && !channel->isGuest(client))
        throw IrcError(sender_nickname, channel_name, CLIENT_BADCHANNELKEY);
    if (channel_settings.l_userLimit && clients.size() >= channel_settings.l_userLimit)
        throw IrcError(sender_nickname, channel_name, CLIENT_CHANNELISFULL);

    channel->addClient(client);

    std::string message;
    message = ":" + sender_nickname + " JOIN " + channel_name + "\r\n";
    channel->broadcastMessage(message, NULL);

    message = ":localhost 353 " + sender_nickname + " = " + channel_name + " :" + getListOfClients(channel) + "\r\n";
    message += ":localhost 366 " + sender_nickname + " " + channel_name + " :End of /NAMES list\r\n";
    message += "Welcome to " + channel_name + ", " + sender_nickname + "\r\n";
    send(client->getFd(), message.c_str(), message.length(), 0);
}

void Command::_executeJoin(Client *client, std::vector<std::string> args)
{
    std::string sender_nickname = client->getNickname();

    if (args.size() < 2)
        throw IrcError(sender_nickname, "JOIN", CLIENT_NEEDMOREPARAMS);

    ChannelPasswordList request_list = initRequestList(args);

    for (ChannelPasswordList::iterator it = request_list.begin(); it != request_list.end(); it++)
    {
		try 
		{
			std::string channel_name = it->first;
			std::string password = it->second;

			Channel *channel = _server->getChannelsList()[channel_name];
			channel ? joinChannel(client, channel, password)
				: createChannel(client, channel_name, password, _server);
		} catch (const IrcError &e)
		{
			e.sendto(*client);
		}
	}
}
