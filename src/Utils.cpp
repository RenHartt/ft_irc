#include <Server.hpp>
#include <Utils.hpp>
#include <regex.h>
#include <sstream>

bool regex(const char *expression, const char *pattern)
{
    regex_t regex;

    if (regcomp(&regex, pattern, REG_EXTENDED)) 
		return false;
    int ret = regexec(&regex, expression, 0, NULL, 0);
    regfree(&regex);
    return (ret == 0);
}

bool isValidChannelName(const std::string &channel_name)
{
    return (regex(channel_name.c_str(), "^[#&][^ ,:]{1,19}$"));
}

bool isValidNickname(const std::string &nickname)
{
    return (regex(nickname.c_str(), "^[a-zA-Z][a-zA-Z0-9\\[\\]\\\\^{}_-]{0,8}$"));
}

bool isValidUsername(const std::string &username)
{
    return (regex(username.c_str(), "^[a-zA-Z0-9\\[\\]\\\\^{}_-]{1,10}$"));
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
