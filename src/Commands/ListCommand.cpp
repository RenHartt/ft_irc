#include "Server.hpp"
#include "Command.hpp"

void Command::_executeList(Client *client, std::vector<std::string>)
{
	std::string message = "List of connected users:\n";
	std::map<int, Client *> clients = _server->getClientsList();
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		message += it->second->getNickname() + "\n";
	}
	send(client->getFd(), message.c_str(), message.size(), 0);

}
