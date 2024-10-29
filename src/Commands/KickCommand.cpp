#include "Utils.hpp"
#include <Client.hpp>
#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <sys/socket.h>

void Command::_executeKick(Client *client, std::vector<std::string> args)
{
    if (args.size() < 3)
        throw IrcError(client->getNickname(), CLIENT_NEEDMOREPARAMS);

    ClientMap  clients_list = _server->getClientsList();
    ChannelMap channels_list = _server->getChannelsList();

    std::vector<std::string> channels = split(args[1], ',');
    std::vector<std::string> recipients = split(args[2], ',');

    for (std::vector<std::string>::iterator it_channel = channels.begin(); it_channel != channels.end(); it_channel++)
    {
        std::string channel = *it_channel;
		for (std::vector<std::string>::iterator it_recipient = recipients.begin(); it_recipient != recipients.end(); it_recipient++)
		{
			std::string recipient = *it_recipient;
			channels_list[channel]->delCLient(clients_list[getFdByNickname(recipient, clients_list)]);
			std::string message = args[3].empty() ? "" : args[3];
			send(getFdByNickname(recipient, clients_list), message.c_str(), message.size(), 0);
		}
    }
}
