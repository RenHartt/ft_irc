#include <Command.hpp>
#include <Client.hpp>
#include <Server.hpp>

#include <iostream>

void Command::_executeStop(Client *, std::vector<std::string>)
{
	running =false;
    std::cout << "STOP" << std::endl;
}
