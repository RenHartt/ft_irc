#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <sys/socket.h>

bool Server::isNicknameAlreadyUsed(const std::string &nickname)
{
    ClientMap::iterator it = _clients_list.begin();
    while (it != _clients_list.end())
        it++;
    return (it->second->getNickname() == nickname);
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
    if (_server->isNicknameAlreadyUsed(args[1]))
        throw IrcError(client->getNickname(), args[1], CLIENT_NICKNAMEINUSE);

    std::string newNickname = args[1];
    client->setNickname(newNickname);

    _server->updateNickname(client->getFd(), newNickname);
    std::string message = "Your nickname is " + client->getNickname() + "\n";
    send(client->getFd(), message.c_str(), message.size(), 0);
}
