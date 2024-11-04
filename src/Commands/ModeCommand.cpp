#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <cstddef>
#include <sys/types.h>

void i_mode(bool adding, Channel *channel) { channel->channel_settings.i_inviteOnly = adding; }

void t_mode(bool adding, Channel *channel)
{
    channel->channel_settings.t_topicRestriction = adding;
}

void k_mode(bool adding, Channel *channel, Client *client, std::vector<std::string> args,
            size_t &arg_index)
{
    std::string client_nickname(client->getNickname());

    if (!channel->isOperator(client))
        throw IrcError(client_nickname, args[1], CLIENT_CHANOPRIVSNEEDED);

    if (adding)
    {
        if (arg_index >= args.size())
            throw IrcError(client_nickname, "k", CLIENT_NEEDMOREPARAMS);
        channel->setPassword(args[arg_index++]);
        channel->channel_settings.k_enableKey = true;
    } else
        channel->channel_settings.k_enableKey = false;
}

void l_mode(bool adding, Channel *channel, Client *client, std::vector<std::string> args,
            size_t &arg_index)
{
    std::string client_nickname(client->getNickname());
    if (adding)
    {
        if (arg_index >= args.size())
            throw IrcError(client_nickname, "l", CLIENT_NEEDMOREPARAMS);
        channel->channel_settings.l_userLimit = std::atoi(args[arg_index++].c_str());
    } else
        channel->channel_settings.l_userLimit = 0;
}

void o_mode(bool adding, Channel *channel, Client *client, std::vector<std::string> args,
            size_t &arg_index, Server *server)
{
    std::string client_nickname(client->getNickname());

    if (!channel->isOperator(client))
        throw IrcError(client_nickname, args[1], CLIENT_CHANOPRIVSNEEDED);

    if (arg_index >= args.size())
        throw IrcError(client_nickname, CLIENT_NEEDMOREPARAMS);

    std::string target_nickname = args[arg_index++];
    Client     *target_client = server->getClientbyNickname(target_nickname);

    if (!target_client || !channel->isMember(target_client))
        throw IrcError(client_nickname, target_nickname, CLIENT_USERNOTINCHANNEL);

    if (adding)
    {
        channel->_clients_rights[target_client->getFd()].rights.isOperator = true;
    } else
    {
        channel->_clients_rights[target_client->getFd()].rights.isOperator = false;
    }
}

void Command::_executeMode(Client *client, std::vector<std::string> args)
{
    std::string client_nickname(client->getNickname());

    if (args.size() < 3)
        throw IrcError(client_nickname, CLIENT_NEEDMOREPARAMS);

    if (!isValidChannelName(args[1]))
        throw IrcError(client_nickname, args[1], CLIENT_BADCHANMASK);

    Channel *channel = _server->getChannelsList()[args[1]];

    if (!channel)
        throw IrcError(client_nickname, args[1], CLIENT_NOSUCHCHANNEL);

    if (channel->isMember(client) == false)
        throw IrcError(client_nickname, args[1], CLIENT_NOTONCHANNEL);

    bool        adding = true;
    std::string modes = args[2];
    size_t      arg_index = 3;

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
                break;
            case 't':
                t_mode(adding, channel);
                break;
            case 'k':
                k_mode(adding, channel, client, args, arg_index);
                break;
            case 'o':
                o_mode(adding, channel, client, args, arg_index, _server);
                break;
            case 'l':
                l_mode(adding, channel, client, args, arg_index);
                break;
            default:
                throw IrcError(client_nickname, std::string(1, modes[i]), CLIENT_UNKNOWNCOMMAND);
            }
        }
    }
}
