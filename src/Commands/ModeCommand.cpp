#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <cstddef>
#include <sys/types.h>

std::string getListOfModes(Channel *channel)
{
    std::string modes_list;

    if (channel->channel_settings.i_inviteOnly)
        modes_list += "i";
    if (channel->channel_settings.k_enableKey)
        modes_list += "k";
    if (channel->channel_settings.t_topicRestriction)
        modes_list += "t";
    if (channel->channel_settings.l_userLimit)
        modes_list += "l " + itoa(channel->channel_settings.l_userLimit);

    return modes_list;
}

void i_mode(bool adding, Channel *channel) { channel->channel_settings.i_inviteOnly = adding; }

void t_mode(bool adding, Channel *channel) { channel->channel_settings.t_topicRestriction = adding; }

void k_mode(bool adding, Channel *channel, std::vector<std::string> args, size_t &arg_index)
{
    if (adding)
    {
        channel->channel_settings.k_enableKey = true;
        channel->setPassword(args[arg_index++]);
    } else
    {
        channel->channel_settings.k_enableKey = false;
        channel->setPassword("");
    }
}

void l_mode(bool adding, Channel *channel, std::vector<std::string> args, size_t &arg_index)
{
    if (adding)
    {
        int limit = atoi(args[arg_index++].c_str());
        channel->channel_settings.l_userLimit = limit;
    } else
        channel->channel_settings.l_userLimit = 0;
}

void o_mode(bool adding, Channel *channel, std::vector<std::string> args, size_t &arg_index, Server *server)
{
    std::string target_nickname = args[arg_index++];
    Client     *target_client = server->getClientByNickname(target_nickname);

    if (!target_client || !channel->isMember(target_client))
        throw IrcError("Channel error", target_nickname, CLIENT_USERNOTINCHANNEL);

    if (adding)
        channel->addOperator(target_client);
    else
        channel->delOperator(target_client);
}

void Command::_executeMode(Client *client, std::vector<std::string> args)
{
    std::string client_nickname(client->getNickname());

    if (args.size() < 2)
        throw IrcError(client_nickname, CLIENT_NEEDMOREPARAMS);

    Channel *channel = _server->getChannelsList()[args[1]];
    std::string channel_name = channel->getChannelName();
    if (args.size() == 2)
    {
        std::string message = ":localhost 324 " + channel_name + " " + getListOfModes(channel) + "\r\n";
        send(client->getFd(), message.c_str(), message.size(), 0);
        return;
    }
    if (!channel)
        throw IrcError(client_nickname, channel_name, CLIENT_NOSUCHCHANNEL);
    if (!channel->isMember(client))
        throw IrcError(client_nickname, channel_name, CLIENT_NOTONCHANNEL);
    if (!channel->isOperator(client))
        throw IrcError(client_nickname, channel_name, CLIENT_CHANOPRIVSNEEDED);

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
                k_mode(adding, channel, args, arg_index);
                modes_applied += (adding ? "+k" : "-k");
                if (adding)
                    parameters.push_back(args[arg_index - 1]);
                break;
            case 'o':
                o_mode(adding, channel, args, arg_index, _server);
                modes_applied += (adding ? "+o" : "-o");
                parameters.push_back(args[arg_index - 1]);
                break;
            case 'l':
                l_mode(adding, channel, args, arg_index);
                modes_applied += (adding ? "+l" : "-l");
                if (adding)
                    parameters.push_back(args[arg_index - 1]);
                break;
            default:
                throw IrcError(client_nickname, std::string(1, modes[i]), CLIENT_UNKNOWNCOMMAND);
            }
        }
    }

    std::string response = ":" + client_nickname + " MODE " + channel_name + " " + modes_applied;
    for (std::vector<std::string>::const_iterator it = parameters.begin(); it != parameters.end(); it++)
        response += " " + *it;
    response += "\r\n";

    channel->broadcastMessage(response, NULL);
}
