#include <sys/types.h>
#include <IrcError.hpp>
#include <Server.hpp>

std::string getListOfModes(Channel *channel)
{
    std::string modes_list;

    ChannelSettings channel_settings = channel->getChannelSettings();

    if (channel_settings.i_inviteOnly)
        modes_list += "i";
    if (channel_settings.k_enableKey)
        modes_list += "k";
    if (channel_settings.t_topicRestriction)
        modes_list += "t";
    if (channel_settings.l_userLimit)
        modes_list += "l " + itoa(channel_settings.l_userLimit);

    return modes_list;
}

void i_mode(bool adding, Channel *channel)
{
    ChannelSettings channel_settings = channel->getChannelSettings();
    channel_settings.i_inviteOnly = adding;
    channel->setChannelSettings(channel_settings);
}

void t_mode(bool adding, Channel *channel)
{
    ChannelSettings channel_settings = channel->getChannelSettings();
    channel_settings.t_topicRestriction = adding;
    channel->setChannelSettings(channel_settings);
}

void k_mode(bool adding, Channel *channel, Client *client, std::vector<std::string> args, size_t &arg_index)
{
    std::string client_nickname = client->getNickname();

    if (adding && arg_index >= args.size())
        throw IrcError(client_nickname, "k", CLIENT_NEEDMOREPARAMS);

    ChannelSettings channel_settings = channel->getChannelSettings();
    channel_settings.k_enableKey = adding;
    adding ? channel->setPassword(args[arg_index++]) : channel->setPassword("");
    channel->setChannelSettings(channel_settings);
}

void l_mode(bool adding, Channel *channel, Client *client, std::vector<std::string> args, size_t &arg_index)
{
    std::string client_nickname = client->getNickname();

    if (adding && arg_index >= args.size())
        throw IrcError(client_nickname, "l", CLIENT_NEEDMOREPARAMS);

    ChannelSettings channel_settings = channel->getChannelSettings();
    channel_settings.l_userLimit = adding ? atoi(args[arg_index++].c_str()) : 0;
    channel->setChannelSettings(channel_settings);
}

void o_mode(bool adding, Channel *channel, Client *client, std::vector<std::string> args, size_t &arg_index, Server *server)
{
    std::string client_nickname = client->getNickname();

    if (arg_index >= args.size())
        throw IrcError(client_nickname, "o", CLIENT_NEEDMOREPARAMS);

    std::string target_nickname = args[arg_index++];
    Client     *target_client = server->getClientByNickname(target_nickname);
    if (!target_client)
        throw IrcError(client_nickname, target_nickname, CLIENT_NOSUCHNICK);
    if (!channel->isMember(target_client))
        throw IrcError(client_nickname, target_nickname, CLIENT_USERNOTINCHANNEL);

    adding ? channel->addOperator(target_client) : channel->delOperator(target_client);
}

void Command::_executeMode(Client *client, std::vector<std::string> &args)
{
    std::string client_nickname(client->getNickname());

    if (args.size() < 2)
        throw IrcError(client_nickname, CLIENT_NEEDMOREPARAMS);

    Channel *channel = _server->getChannelsList()[args[1]];
    if (!channel)
        throw IrcError(client_nickname, args[1], CLIENT_NOSUCHCHANNEL);
    if (!channel->isMember(client))
        throw IrcError(client_nickname, args[1], CLIENT_NOTONCHANNEL);
    if (args.size() == 2)
    {
        std::string message = ":localhost 324 " + args[1] + " " + getListOfModes(channel) + "\r\n";
        send(client->getFd(), message.c_str(), message.size(), 0);
        return;
    }
    if (!channel->isOperator(client))
        throw IrcError(client_nickname, args[1], CLIENT_CHANOPRIVSNEEDED);

    bool                     adding = true;
    std::string              modes = args[2];
    std::size_t              arg_index = 3;
    std::string              modes_applied;
    std::vector<std::string> parameters;

    for (size_t i = 0; i < modes.size(); i++)
    {
        if (modes[i] == '+')
            adding = true;
        else if (modes[i] == '-')
            adding = false;
        else
        {
            switch (modes[i])
            {
            case 'i':
                i_mode(adding, channel);
                modes_applied += (adding ? "+i" : "-i");
                break;
            case 't':
                t_mode(adding, channel);
                modes_applied += (adding ? "+t" : "-t");
                break;
            case 'k':
                k_mode(adding, channel, client, args, arg_index);
                modes_applied += (adding ? "+k" : "-k");
                if (adding)
                    parameters.push_back(args[arg_index - 1]);
                break;
            case 'o':
                o_mode(adding, channel, client, args, arg_index, _server);
                modes_applied += (adding ? "+o" : "-o");
                parameters.push_back(args[arg_index - 1]);
                break;
            case 'l':
                l_mode(adding, channel, client, args, arg_index);
                modes_applied += (adding ? "+l" : "-l");
                if (adding)
                    parameters.push_back(args[arg_index - 1]);
                break;
            default:
                throw IrcError(client_nickname, std::string(1, modes[i]), CLIENT_UNKNOWNCOMMAND);
            }
        }
    }

    std::string message = ":" + client_nickname + " MODE " + args[1] + " " + modes_applied;
    for (std::vector<std::string>::const_iterator it = parameters.begin(); it != parameters.end(); it++)
        message += " " + *it;
    message += "\r\n";

    channel->broadcastMessage(message, NULL);
}
