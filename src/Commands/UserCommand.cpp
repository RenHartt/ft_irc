#include <Command.hpp>
#include <Client.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <IrcError.hpp>

void Command::_executeUser(Client* client, std::vector<std::string> args)
{
    if (args.size() < 3)
		throw IrcError(client->getNickname(), CLIENT_NEEDMOREPARAMS);

    if (client->getIsRegistered())
		throw IrcError(client->getNickname(), CLIENT_ALREADYREGISTERED);

    std::string username = args[1], realname = args[2];

    if (realname[0] == ':')
        realname = realname.substr(1);

    client->setUsername(username);
    client->setRealname(realname);
    client->setIsRegistered(true);
}
