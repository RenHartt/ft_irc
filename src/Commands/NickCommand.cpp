#include <Command.hpp>
#include <Client.hpp>
#include <Server.hpp>
#include <iostream>

void Server::updateNickname(int client_fd, const std::string &new_nickname)
{
    Client *client = _clients_list[client_fd];

    for (ClientMap::iterator it = _clients_list.begin(); it != _clients_list.end(); it++)
    {
        if (it->second == client)
        {
            _clients_list.erase(it);
            break;
        }
    }

    client->setNickname(new_nickname);
    _clients_list[client_fd] = client;
    std::cout << "Client with FD " << client_fd << " has taken nickname: " << new_nickname << std::endl;
}

void Command::_executeNick(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        std::string message = "Usage : NICK <nickname>\n";
        send(client->getFd(), message.c_str(), message.size(), 0);
        std::cout << "Please enter a nickname" << std::endl;
    }
    else
    {
        std::string newNickname = args[1]; 
        client->setNickname(newNickname); 

        std::string message = "Your nickname is " + client->getNickname() + "\n";
		_server->updateNickname(client->getFd(), newNickname);
        send(client->getFd(), message.c_str(), message.size(), 0);
    }
}
