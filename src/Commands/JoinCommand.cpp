#include <Command.hpp>
#include <Client.hpp>

#include <iostream>

void Command::_executeJoin(Client *client, std::vector<std::string>)
{
    std::cout << "JOIN : " << client->getFd() << std::endl;
}
