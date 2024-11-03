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

    std::string modes(args[2]);
    bool        adding = true;
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
#include <ChannelPermissionList.hpp> // yes
            default:
                throw IrcError(client_nickname, std::string(1, mode), CLIENT_UNKNOWNCOMMAND);
                break;
            }
        }
    }
}
