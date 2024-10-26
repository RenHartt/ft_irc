#include <Command.hpp>
#include <Client.hpp>
#include <Server.hpp>
#include <Utils.hpp>

void Command::_executeUser(Client *sender, std::vector<std::string>)
{
    std::string message = "NickName -> " + sender->getNickname() + "\n";
    send(sender->getFd(), message.c_str(), message.size(), 0);
    message = "fd -> " + itoa(sender->getFd()) + "\n";
    send(sender->getFd(), message.c_str(), message.size(), 0);
}



