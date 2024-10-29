#include <Command.hpp>
#include <Server.hpp>

void Command::_executeWhoami(Client *client, std::vector<std::string>)
{
    std::string message = "Your nickname is " + client->getNickname() + "\n";
    send(client->getFd(), message.c_str(), message.size(), 0);
}
