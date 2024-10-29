#include <Command.hpp>
#include <Server.hpp>

#include <iostream>

void Command::_executePart(Client *client, std::vector<std::string>)
{
    std::cout << "PART : " << client->getFd() << std::endl;
}
