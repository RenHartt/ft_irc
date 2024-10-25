#include <Command.hpp>
#include <Client.hpp>
#include <Server.hpp>

void Command::_executeJoin(Client *client, std::vector<std::string> args)
{
	std::map<std::string, Channel *> channels_list = _server->getChannelsList();
    std::map<std::string, Channel *>::iterator it = channels_list.find(args[1]);
    if (it == channels_list.end())
    {
        channels_list[args[1]] = new Channel(args[1]);
        it = channels_list.find(args[1]);
    }

    it->second->addClient(client);

    std::string welcomeMessage = "Welcome to " + args[1] + ", " + client->getNickname() + "\r\n";
    send(client->getFd(), welcomeMessage.c_str(), welcomeMessage.length(), 0);
}
