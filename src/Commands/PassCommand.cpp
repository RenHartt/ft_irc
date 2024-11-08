#include <Command.hpp>
#include <ErrTable.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <cstring>

void Command::_executePass(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2 || args[1].empty())
        throw IrcError(client->getNickname(), "PASS", CLIENT_NEEDMOREPARAMS);
    else if (!_server->checkPassword(args[1]))
        throw IrcError(client->getNickname(), CLIENT_PASSWDMISMATCH);
    else
	{
		client->setIsAuthenticated(true);

		std::string success_message = "001 " + client->getNickname() + " :Welcome to the IRC server\r\n";
		send(client->getFd(), success_message.c_str(), success_message.size(), 0);
    }
}
