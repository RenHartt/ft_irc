#include <Channel.hpp>
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
    for (ClientMap::iterator it = channel->clients.begin(); it != channel->clients.end(); it++)
    {
        channel->isOperator(it->second) ? clients_list += "@" + it->second->getNickname()
                                        : clients_list += it->second->getNickname();

        if (it != channel->clients.end())
            clients_list += " ";
    }

    return clients_list;
}

std::string getListOfModes(Channel *channel)
{
    std::string modes_list;

    channel->channel_settings.i_inviteOnly ? modes_list += "i" : modes_list += "";
    channel->channel_settings.k_enableKey ? modes_list += "k" : modes_list += "";
    channel->channel_settings.t_topicRestriction ? modes_list += "t" : modes_list += "";
    channel->channel_settings.l_userLimit
        ? modes_list += "l " + itoa(channel->channel_settings.l_userLimit)
        : modes_list += "";
    return modes_list;
}

void createChannel(Server *server, Client *client, const std::string &channel_name, const std::string &password)
{
    if (isValidChannelName(channel_name) == false)
        throw IrcError(client->getNickname(), channel_name, CLIENT_BADCHANMASK);

    Channel *newChannel;
    password.empty() ? newChannel = new Channel(channel_name)
                     : newChannel = new Channel(channel_name, password);

    server->addChannel(channel_name, newChannel);
    newChannel->addClient(client);
    newChannel->addOperator(client);

    std::string message;
    message = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " JOIN " + channel_name + "\r\n";
    newChannel->broadcastMessage(message, NULL);

    message = ":localhost 353 " + client->getNickname() + " = " + channel_name + " :@" + client->getNickname() + "\r\n";
    message += ":localhost 366 " + client->getNickname() + " " + channel_name + " :End of /NAMES list\r\n";
    send(client->getFd(), message.c_str(), message.length(), 0);
}

void joinChannel(Client *client, Channel *channel)
{
    channel->addClient(client);

    std::string message;
    message = ":" + client->getNickname() + " JOIN " + channel->getChannelName() + "\r\n";
    channel->broadcastMessage(message, NULL);

    message = ":localhost 353 " + client->getNickname() + " = " + channel->getChannelName() + " :" + getListOfClients(channel) + "\r\n";
    message += ":localhost 366 " + client->getNickname() + " " + channel->getChannelName() + " :End of /NAMES list\r\n";
    message += "Welcome to " + channel->getChannelName() + ", " + client->getNickname() + "\r\n";
    send(client->getFd(), message.c_str(), message.length(), 0);
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

    if (args.size() < 2)
        throw IrcError(client->getNickname(), "JOIN", CLIENT_NEEDMOREPARAMS);

    ChannelPasswordList           request_list = initRequestList(args);
    ChannelPasswordList::iterator it = request_list.begin();

    for (; it != request_list.end(); it++)
    {
        std::string channel_name = it->first, password = it->second;

        ChannelMap           channels_list = _server->getChannelsList();
        ChannelMap::iterator it_channel = channels_list.find(channel_name);
        Channel             *channel = it_channel->second;

        if (it_channel == channels_list.end())
            createChannel(_server, client, channel_name, password);
        else if (channel->isGuest(client) == true)
            joinChannel(client, channel);
        else if (channel->channel_settings.i_inviteOnly == true && channel->isGuest(client) == false)
            throw IrcError(client->getNickname(), channel_name, CLIENT_INVITEONLYCHAN);
        else if (channel->channel_settings.k_enableKey == true && channel->getPassword() != password)
            throw IrcError(client->getNickname(), channel_name, CLIENT_BADCHANNELKEY);
        else if (channel->channel_settings.l_userLimit == true && channel->channel_settings.l_userLimit == channel->clients.size())
            throw IrcError(client->getNickname(), channel_name, CLIENT_CHANNELISFULL);
        else
            joinChannel(client, channel);
    }
}
