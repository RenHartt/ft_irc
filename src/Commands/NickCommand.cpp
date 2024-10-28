#include <Command.hpp>
#include <Client.hpp>
#include <Server.hpp>
#include <iostream>
#include <IrcError.hpp>
#include <sys/socket.h>

bool Server::NicknameAlreadyUsed(const std::string &nickname)
{
	ClientMap::iterator it = _clients_list.begin();
	while (it != _clients_list.end())
	{
		if (it->second->getNickname() == nickname)
			return true;
		it++;
	}
	return false;
}

void Server::updateNickname(int client_fd, const std::string &new_nickname)
{
	_clients_list[client_fd]->setNickname(new_nickname);

    std::cout << "Client with FD " << client_fd << " has taken nickname: " << new_nickname << std::endl;
}

void Command::_executeNick(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2)
		throw IrcError(client->getNickname(), CLIENT_NONICKNAMEGIVEN);
	if (_server->NicknameAlreadyUsed(args[1]))
		throw IrcError(client->getNickname(), args[1], CLIENT_NICKNAMEINUSE);

	std::string newNickname = args[1]; 
	client->setNickname(newNickname); 

	_server->updateNickname(client->getFd(), newNickname);
	std::string message = "Your nickname is " + client->getNickname() + "\n";
	send(client->getFd(), message.c_str(), message.size(), 0);
}
