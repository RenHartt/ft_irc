#include "Utils.hpp"
#include <Server.hpp>
#include <sstream>

bool isValidChannelName(const std::string &channel_name)
{
    return (channel_name[0] == '#' || channel_name[0] == '&');
}

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
        splited.push_back(token);

    if (!trailing.empty())
        splited.push_back(trailing);

    return splited;
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream       iss(str);
    std::string              token;

    while (std::getline(iss, token, delimiter))
        tokens.push_back(token);

    return tokens;
}

std::string itoa(int value)
{
    std::stringstream ss;
    ss << value;
    return (ss.str());
}

int getFdByNickname(const std::string &nickname, ClientMap clients_list)
{
    for (ClientMap::iterator it = clients_list.begin(); it != clients_list.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return it->first;
    }
    return -1;
}
