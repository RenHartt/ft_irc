#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>

void Command::_executeUser(Client *client, std::vector<std::string> args)
{
	std::string sender_nickname = client->getNickname();

    if (args.size() < 2)
        throw IrcError(sender_nickname, CLIENT_NEEDMOREPARAMS);
    if (client->getIsRegistered())
        throw IrcError(sender_nickname, CLIENT_ALREADYREGISTERED);

    client->setUsername(args[1]);

    if (sender_nickname.empty() == false)
	{
		client->setIsRegistered(true);
		std::string message = ":localhost 001 " + sender_nickname + " :Welcome to the IRC server\r\n";
		send(client->getFd(), message.c_str(), message.size(), 0);
	}
}
