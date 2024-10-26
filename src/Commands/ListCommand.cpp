#include "Server.hpp"
#include "Command.hpp"

void Command::_executeList(Client *client, std::vector<std::string>)
{
	std::string list = "Channel List: \n";
	ChannelMap channel_list = _server->getChannelsList();
	ChannelMap::iterator it = channel_list.begin();
	while (it != channel_list.end())
	{
		list += it->first + "\n";
		it++;
	}	
	send(client->getFd(), list.c_str(), list.size(), 0);	
}
