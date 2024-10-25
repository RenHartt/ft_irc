#include "Utils.hpp"
#include <Server.hpp>
#include <sstream>

std::vector<std::string> Server::splitCommand(const char *buffer)
{
    std::vector<std::string> splited;
    std::string              command(buffer);
    std::size_t              colonPos = command.find(':');
    std::string              trailing;

    if (colonPos != std::string::npos)
    {
        trailing = command.substr(colonPos + 1);
        command = command.substr(0, colonPos);
    }

    std::stringstream ss(command);
    std::string       token;

    while (ss >> token)
    {
        splited.push_back(token);
    }
    if (!trailing.empty())
    {
        splited.push_back(trailing);
    }

    return (splited);
}

std::string itoa(int value)
{
    std::stringstream ss;
    ss << value;
    return (ss.str());
}

int getFdByNickname(const std::string &nickname, std::map<int, Client *> clients_list)
{
	for (std::map<int, Client *>::iterator it = clients_list.begin(); it != clients_list.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return it->first;
	}
	return -1;
}
