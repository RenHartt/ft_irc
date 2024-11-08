#include <Channel.hpp>
#include <Client.hpp>
#include <Command.hpp>
#include <ErrTable.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>

std::string getListOfClients(Channel *channel)
{
	std::string clients_list;
	ClientMap::iterator it = channel->
	for (; it != channel->clients.end(); it++)
		clients_list += it->first->getNickname() + " ";
	 
}

void createChannel(Server *server, Client *client, const std::string &channel_name,
                             const std::string &password)
{
    if (isValidChannelName(channel_name) == false)
        throw IrcError(client->getNickname(), channel_name, CLIENT_BADCHANMASK);

    Channel *newChannel = new Channel(channel_name, password);

    server->addChannel(channel_name, newChannel);
    newChannel->addClient(client, true);
	
    std::string message = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost"+ " JOIN " + channel_name + "\r\n";
	newChannel->broadcastMessage(message, client);
	std::string mode_msg = ":localhost MODE " + channel_name + " +n\r\n";
	mode_msg += ":localhost 353 " + client->getNickname() + " = " + channel_name + " :@" + client->getNickname() + "\r\n";
	mode_msg += ":localhost 366 " + client->getNickname() + " " + channel_name + " :End of /NAMES list\r\n";
	send(client->getFd(), mode_msg.c_str(), mode_msg.length(), 0);

}

void joinChannel(Client *client, Channel *channel)
{
    channel->addClient(client, false);

    std::string join_message = ":" + client->getNickname() + " JOIN " + channel->getChannelName() + "\r\n";
    channel->broadcastMessage(join_message, client);
}

ChannelPasswordList initRequestList(std::vector<std::string> args)
{
    ChannelPasswordList      request_list;
    std::vector<std::string> channels_list, password_list;

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

void Command::_executeJoin(Client *client, std::vector<std::string> args)
{

    if (args.size() < 2 || args[1].empty())
    {
        std::string nickname = client->getNickname().empty() ? "*" : client->getNickname();
        throw IrcError(nickname, "JOIN", CLIENT_NEEDMOREPARAMS);
    }

    ChannelPasswordList request_list = initRequestList(args);

    for (ChannelPasswordList::iterator it = request_list.begin(); it != request_list.end(); it++)
    {
        std::string channel_name = it->first, password = it->second;

        ChannelMap           channels_list = _server->getChannelsList();
        ChannelMap::iterator it_channel = channels_list.find(channel_name);
		Channel             *channel = it_channel->second;

		if (it_channel == channels_list.end())
			createChannel(_server, client, channel_name, password);
		else if (channel->channel_settings.i_inviteOnly == true)
			throw IrcError(client->getNickname(), channel_name, CLIENT_INVITEONLYCHAN);
		else if (channel->getPassword() != password && channel->channel_settings.k_enableKey == true)
			throw IrcError(client->getNickname(), channel_name, CLIENT_BADCHANNELKEY);
		else if (channel->clients_rights.size() == channel->channel_settings.l_userLimit && channel->channel_settings.l_userLimit)
			throw IrcError(client->getNickname(), channel_name, CLIENT_CHANNELISFULL);
		else
		{
			joinChannel(client, channel);

			std::string welcome_message = "Welcome to " + channel_name + ", " + client->getNickname() + "\r\n";
			send(client->getFd(), welcome_message.c_str(), welcome_message.length(), 0);
		}
	}
}
