#include <Channel.hpp>
#include <Server.hpp>

Channel::Channel(std::string) {}

void Channel::addClient(Client *client)
{
    ClientRight defaultRight;
    _clients_rights[client->getFd()] = defaultRight;
}

void Channel::broadcastMessage(const std::string &message, Client *sender)
{
    for (std::map<int, ClientRight>::iterator it = _clients_rights.begin();
         it != _clients_rights.end(); it++)
    {
        int fd = it->first;
        if (fd != sender->getFd())
        {
            send(fd, message.c_str(), message.length(), 0);
        }
    }
}
