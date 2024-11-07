#include <Client.hpp>
#include <Command.hpp>
#include <Server.hpp>
#include <csignal>
#include <iostream>

extern sig_atomic_t server_running;

void Command::_executeDie(Client *, std::vector<std::string>)
{
    server_running = false;
    std::cout << "STOP" << std::endl;
}
