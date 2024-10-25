#include <Channel.hpp>
#include <Client.hpp>
#include <Command.hpp>
#include <Server.hpp>

void Command::_createChannel(Client *client, const std::string &channel_name)
{
    Channel *newChannel = new Channel(channel_name);
    newChannel->addClient(client);
    
    _server->addChannel(channel_name, newChannel);
    
    std::string message =
        ":" + client->getNickname() + " JOIN " + channel_name + "\r\n";
    newChannel->broadcastMessage(message, client);
}

void Command::_joinChannel(Client *client, Channel *channel)
{
    channel->addClient(client);

    std::string join_message =
        ":" + client->getNickname() + " JOIN " + channel->getChannelName() + "\r\n";
    channel->broadcastMessage(join_message, client);
}

void Command::_executeJoin(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2 || args[1].empty())
    {
        std::string error_message = "Error: No channel name provided.\r\n";
        send(client->getFd(), error_message.c_str(), error_message.length(), 0);
        return;
    }

	for (std::vector<std::string>::iterator it_args = args.begin() + 1; it_args != args.end(); it_args++)
	{
		std::string channel_name = *it_args;

		std::map<std::string, Channel *> channels_list = _server->getChannelsList();
		std::map<std::string, Channel *>::iterator it_channel =
			channels_list.find(channel_name);
		if (it_channel == channels_list.end())
			_createChannel(client, channel_name);
		else
			_joinChannel(client, it_channel->second);

		std::string welcome_message =
			"Welcome to " + channel_name + ", " + client->getNickname() + "\r\n";
		send(client->getFd(), welcome_message.c_str(), welcome_message.length(), 0);
	}
}
