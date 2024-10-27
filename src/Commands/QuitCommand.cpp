#include <Command.hpp>
#include <Server.hpp>
#include <iostream>

void Command::_executeQuit(Client *client, std::vector<std::string>)
{
    std::string quit_message = ":" + client->getNickname() + " QUIT :Client disconnected\r\n";
    ClientMap clients_list = _server->getClientsList();
    for (ClientMap::iterator it = clients_list.begin(); it != clients_list.end(); it++)
    {
        if (it->second != client) 
            send(it->second->getFd(), quit_message.c_str(), quit_message.size(), 0);
    }

    close(client->getFd());

    std::vector<pollfd> fds = _server->getPollFds(); 
    for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end();)
    {
        if (it->fd == client->getFd())
            it = fds.erase(it); 
        else
            it++; 
    }

    clients_list.erase(client->getFd());
    delete client;

    std::cout << quit_message << std::endl;
}
