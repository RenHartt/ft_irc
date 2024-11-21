#include <csignal>
#include <Client.hpp>
#include <Command.hpp>
#include <Server.hpp>

extern sig_atomic_t server_running;

void Command::_executeDie(Client *, std::vector<std::string> &)
{
    server_running = false;
}
