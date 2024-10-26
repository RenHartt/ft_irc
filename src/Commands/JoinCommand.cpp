#include <Utils.hpp>
#include <Channel.hpp>
#include <Client.hpp>
#include <Command.hpp>
#include <Server.hpp>
#include <ErrTable.hpp>

void Command::_createChannel(Client *client, const std::string &channel_name, const std::string &password)
{
    Channel *newChannel = new Channel(channel_name, password);
    newChannel->addClient(client);

    _server->addChannel(channel_name, newChannel);

    std::string message = ":" + client->getNickname() + " JOIN " + channel_name + "\r\n";
    newChannel->broadcastMessage(message, client);
}

void Command::_joinChannel(Client *client, Channel *channel)
{
    channel->addClient(client);

    std::string join_message = ":" + client->getNickname() + " JOIN " + channel->getChannelName() + "\r\n";
    channel->broadcastMessage(join_message, client);
}

ChannelPasswordList initRequestList(std::vector<std::string> args)
{
    ChannelPasswordList request_list;
	std::vector<std::string> channels_list, password_list;

	if (args.size() > 1)
		channels_list = split(args[1], ',');
	if (args.size() > 2)
		password_list = split(args[2], ',');

    for (std::size_t i = 0; i < channels_list.size(); ++i)
    {
        std::string password = (i < password_list.size()) ? password_list[i] : "";
		request_list.push_back(std::make_pair(channels_list[i], password));
    }

    return request_list;
}

void Command::_executeJoin(Client *client, std::vector<std::string> args)
{

    if (args.size() < 2 || args[1].empty())
    {
        std::string nickname = client->getNickname().empty() ? "*" : client->getNickname();
        std::string error_message = ERR_NEEDMOREPARAMS(nickname, "JOIN") + "\r\n";
        send(client->getFd(), error_message.c_str(), error_message.length(), 0);
        return;
    }

    ChannelPasswordList request_list = initRequestList(args);

    for (ChannelPasswordList::iterator it = request_list.begin(); it != request_list.end(); it++)
    {
        std::string channel_name = it->first, password = it->second;

        ChannelMap channels_list = _server->getChannelsList();
        ChannelMap::iterator it_channel = channels_list.find(channel_name);

        if (it_channel == channels_list.end())
            _createChannel(client, channel_name, password);
        else if (it_channel->second->getPassword() == password)
        {
            Channel *channel = it_channel->second;
            _joinChannel(client, channel);
            std::string welcome_message = "Welcome to " + channel_name + ", " + client->getNickname() + "\r\n";
            send(client->getFd(), welcome_message.c_str(), welcome_message.length(), 0);
        } else
        {
            std::string error_message = "Error: Incorrect password for channel " + channel_name + ".\r\n";
            send(client->getFd(), error_message.c_str(), error_message.length(), 0);
            continue;
        }
    }
}
