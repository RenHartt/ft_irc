#include <Command.hpp>
#include <ErrTable.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <cstring>

void Command::_executePass(Client *client, std::vector<std::string> args)
{
    std::string sender_nickname = client->getNickname();

    if (args.size() < 2)
        throw IrcError(sender_nickname, "PASS", CLIENT_NEEDMOREPARAMS);
    if (!_server->checkPassword(args[1]))
        throw IrcError(sender_nickname, CLIENT_PASSWDMISMATCH);

    client->setIsAuthenticated(true);
}
