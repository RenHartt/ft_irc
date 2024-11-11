#include <IrcError.hpp>
#include <Server.hpp>

bool NicknameAlreadyUsed(const std::string &nickname, ClientMap &clients_list)
{
    for (ClientMap::const_iterator it = clients_list.begin(); it != clients_list.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return true;
    }
    return false;
}

void Command::_executeNick(Client *client, std::vector<std::string> &args)
{
    std::string old_nickname = client->getNickname();
	ClientMap clients_list = _server->getClientsList();

    if (args.size() < 2)
        throw IrcError(client->getNickname(), CLIENT_NONICKNAMEGIVEN);
    if (isValidNickname(args[1]) == false)
        throw IrcError(client->getNickname(), CLIENT_ERRONEUSNICKNAME);
    if (NicknameAlreadyUsed(args[1], clients_list) == true)
        throw IrcError(client->getNickname(), args[1], CLIENT_NICKNAMEINUSE);

    client->setNickname(args[1]);
	std::string message;

    if (client->getUsername().empty() == false && client->getIsRegistered() == false)
	{
		client->setIsRegistered(true);
		message = ":localhost 001 " + client->getNickname() + " :Welcome to the IRC server\r\n";
		send(client->getFd(), message.c_str(), message.size(), 0);
	} else
	{
		message = ":" + old_nickname + " NICK " + client->getNickname() + "\r\n";
		_server->broadcastServer(message);
	}
}
