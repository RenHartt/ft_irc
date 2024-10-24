#include <Command.hpp>
#include <Server.hpp>

void Command::_executeQuit(Client *client, std::vector<std::string>)
{
    close(client->getFd());

	std::vector<pollfd> fds = _server->getPollFds();
    for (std::vector<pollfd>::iterator it = fds.begin();
         it != fds.end();)
    {
        it->fd == client->getFd() ? it = fds.erase(it) : it++;
    }
    _server->getClientsList().erase(itoa(client->getFd()));
    delete client;

    std::cout << "Client deconnecte" << std::endl;
}
