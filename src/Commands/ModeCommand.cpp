#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <cstddef>

#define SET_PERMISSION(name)                                                                       \
    case #name[0]:                                                                                 \
        std::cout << #name << std::endl;                                                           \
        channel->channel_settings.name = adding;                                                   \
        break;

#define SET_PERMISSION_ADMIN(name)                                                                 \
    case #name[0]:                                                                                 \
        channel->channel_settings.admin.name = adding;                                             \
        break;

#define SET_PERMISSION_ARGNUM(name)                                                                \
    case #name[0]:                                                                                 \
        channel->channel_settings.argNum.name = adding;                                            \
        break;

void Command::_executeMode(Client *client, std::vector<std::string> args)
{
    std::string client_nickname(client->getNickname());

    if (args.size() < 2)
        throw IrcError(client_nickname, CLIENT_NEEDMOREPARAMS);
    if (isValidChannelName(args[1]) == false)
        throw IrcError(client_nickname, args[1], CLIENT_BADCHANMASK);

    ChannelMap channels_list = _server->getChannelsList();
    Channel   *channel = channels_list[args[1]];

    if (!channel)
        throw IrcError(client_nickname, args[1], CLIENT_NOSUCHCHANNEL);

    bool        adding = true;
    std::string modes(args[2]);
    for (std::size_t i = 0; i < modes.length(); i++)
    {
        char mode = modes[i];
        if (mode == '+')
            adding = true;
        else if (mode == '-')
            adding = false;
        else
        {
            switch (mode)
            {
				/* case 'i': */
				/* 	channel->channel_settings.i_inviteOnly = adding;                                                   \ */
				/* case 't' : */
				/* 	channel->channel_settings.name = adding;                                                   \ */
				/* case 'k' : */
				/* 	channel->channel_settings.name = adding;                                                   \ */
				/* case 'o' : */
				/* 	channel->channel_settings.name = adding;                                                   \ */
				/* case 'l' : */
				/* 	channel->channel_settings.name = adding;                                                   \ */
#include <ChannelPermissionList.hpp> // yes
            default:
                throw IrcError(client_nickname, std::string(1, mode), CLIENT_UNKNOWNCOMMAND);
                break;
            }
        }
    }
}
