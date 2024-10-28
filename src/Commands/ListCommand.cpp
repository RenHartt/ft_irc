#include "Server.hpp"
#include "Command.hpp"
#include "IrcError.hpp"
#include "ErrTable.hpp"

#include <sstream> 

void Command::_executeList(Client *client, std::vector<std::string>)
{
    std::ostringstream oss;
    std::string list;
    ChannelMap channel_list = _server->getChannelsList();
    ChannelMap::iterator it = channel_list.begin();

    std::string server_name = _server->getName();
    std::string client_nickname = client->getNickname();

    while (it != channel_list.end())
    {
        Channel *channel = it->second;
        int member_count = _server->getClientCount();
        std::string topic = channel->list_channel(channel);

        oss.str("");
        oss << member_count;
        list += "322 " + server_name + " " + client_nickname + " " + channel->getChannelName() + " "
              + oss.str() + " :" + topic + "\r\n";
        ++it;
    }
	list += RPL_LISTEND(server_name, client_nickname);
    send(client->getFd(), list.c_str(), list.size(), 0);
}

