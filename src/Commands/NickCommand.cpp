#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <sys/socket.h>

bool Server::NicknameAlreadyUsed(const std::string &nickname)
{
    for (ClientMap::iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return true;
    }
    return false;
}

void Server::updateNickname(int client_fd, const std::string &new_nickname)
{
    _clients_list[client_fd]->setNickname(new_nickname);
}

void Command::_executeNick(Client *client, std::vector<std::string> args)
{
    std::string old_nickname = client->getNickname();

    if (args.size() < 2)
        throw IrcError(client->getNickname(), CLIENT_NONICKNAMEGIVEN);
    if (isValidNickname(args[1]) == false)
        throw IrcError(client->getNickname(), CLIENT_ERRONEUSNICKNAME);
    if (_server->NicknameAlreadyUsed(args[1]))
    {
        client->failed_nick = args[1];
        throw IrcError(client->getNickname(), args[1], CLIENT_NICKNAMEINUSE);
    }

    client->setNickname(args[1]);
    _server->updateNickname(client->getFd(), args[1]);

    if (client->getUsername().empty() == false)
        client->setIsRegistered(true);
    if (client->failed_nick.empty() == false)
    {
        std::string message = ":" + client->failed_nick + " NICK " + client->getNickname() + "\r\n";
		client->failed_nick = "";
		send(client->getFd(), message.c_str(), message.size(), 0);
    } else if (old_nickname.empty() == false)
    {
        std::string message = ":" + old_nickname + " NICK " + client->getNickname() + "\r\n";
        _server->broadcastServer(message);
    }
}
