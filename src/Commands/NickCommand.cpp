#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
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
}

void Command::_executeNick(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2)
        throw IrcError(client->getNickname(), CLIENT_NONICKNAMEGIVEN);
    if (isValidNickname(args[1]) == false)
        throw IrcError(client->getNickname(), CLIENT_ERRONEUSNICKNAME);
    if (_server->NicknameAlreadyUsed(args[1]))
        throw IrcError(client->getNickname(), args[1], CLIENT_NICKNAMEINUSE);

    client->setNickname(args[1]);
    _server->updateNickname(client->getFd(), args[1]);

    std::string message = "Your nickname is " + client->getNickname() + "\n";
    send(client->getFd(), message.c_str(), message.size(), 0);
}
