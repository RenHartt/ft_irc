#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>

void Command::_executeUser(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2)
        throw IrcError(client->getNickname(), CLIENT_NEEDMOREPARAMS);
    if (client->getIsRegistered())
        throw IrcError(client->getNickname(), CLIENT_ALREADYREGISTERED);

    std::string username = args[1];

    client->setUsername(username);
    if (client->getNickname().empty() == false)
	{
		client->setIsRegistered(true);
		std::string success_message = ":localhost 001 " + client->getNickname() + " :Welcome to the IRC server\r\n";
		send(client->getFd(), success_message.c_str(), success_message.size(), 0);
	}
}
