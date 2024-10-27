#include <Command.hpp>
#include <Client.hpp>
#include <Server.hpp>
#include <Utils.hpp>

void Command::_executeUser(Client* client, std::vector<std::string> args)
{
    if (args.size() < 3)
    {
        std::string error_message = "461 " + client->getNickname() + " USER :Not enough parameters\r\n";
        send(client->getFd(), error_message.c_str(), error_message.size(), 0);
        return;
    }

    if (client->getIsRegistered())
    {
        std::string error_message = "462 " + client->getNickname() + " :You may not reregister\r\n";
        send(client->getFd(), error_message.c_str(), error_message.size(), 0);
        return;
    }

    std::string username = args[1];
    std::string realname = args[2];

    if (realname[0] == ':')
        realname = realname.substr(1);

    client->setUsername(username);
    client->setRealname(realname);
    client->setIsRegistered(true);
}
