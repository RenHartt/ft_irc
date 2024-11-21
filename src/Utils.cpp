#include <Utils.hpp>
#include <csignal>
#include <regex.h>
#include <sstream>
#include <iostream>

extern sig_atomic_t server_running;

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
    return (regex(channel_name.c_str(), "^#[^ ,:]{1,1024}$"));
}

bool isValidNickname(const std::string &nickname)
{
    return (regex(nickname.c_str(), "^[a-zA-Z][][a-zA-Z0-9\\^{}_-]{0,1023}$"));
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

void handleSignal(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "\nServeur interrompu par SIGINT (Ctrl+C). Fermeture en cours...\n";
		server_running = 0;
	}
}
