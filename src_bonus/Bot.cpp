#include "Bot.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

Bot::Bot(const std::string &server_address, int port, const std::string &password)
    : _server_address(server_address),
      _port(port),
      _password(password)
{
    initHelpMap();
}

Bot::~Bot(void) {}

HelpMap Bot::getMap(void) const { return _help_map; }
void    Bot::setMap(const HelpMap &help_map) { _help_map = help_map; }

void Bot::initHelpMap(void)
{
    _help_map["!join"]    = "Usage join: /join <password>";
    _help_map["!mode"]    = "Usage mode: /mode <channel> {[+|-]i|t|k|o|l} [<password>] [<user>] [<limit>] ";
    _help_map["!mode i"]  = "Usage mode: /mode <channel> [+|-]i";
    _help_map["!mode t"]  = "Usage mode: /mode <channel> [+|-]t";
    _help_map["!mode k"]  = "Usage mode: /mode <channel> [+|-]k [<password>]";
    _help_map["!mode o"]  = "Usage mode: /mode <channel> [+|-]o <user>";
    _help_map["!mode l"]  = "Usage mode: /mode <channel> [+|-]l [<limit>] ";
	_help_map["!part"]    = "Usage part: /part <channel>{,<channel>}";
    _help_map["!kick"]    = "Usage kick: /kick <channel> <user> [<comment>]";
    _help_map["!topic"]   = "Usage topic: /topic <channel> [<topic>]";
    _help_map["!invite"]  = "Usage invite: /invite <nickname> <channel>";
    _help_map["!privmsg"] = "Usage privmsg: /privmsg <receiver>{,<receiver>} <text to be sent>";
    _help_map["!die"]     = "Usage dir: /die";
}

void Bot::connectToServer(void)
{
    if ((_bot_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Failed to create socket");

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(_port);

    if (inet_pton(AF_INET, _server_address.c_str(), &address.sin_addr) <= 0)
        throw std::runtime_error("Invalid server address");
    if (connect(_bot_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::runtime_error("Failed to connect to server");

    _bot_pollfd.fd = _bot_fd;
    _bot_pollfd.events = POLLIN;
    _bot_pollfd.revents = 0;
}

void Bot::authenticate(void)
{
    std::string command;
    command += "PASS " + _password + "\r\n";
    command += "NICK bot\r\n";
    command += "USER bot 0 * :Bot Real Name\r\n";
    if (send(_bot_fd, command.c_str(), command.length(), 0) < 0)
        throw std::runtime_error("Failed to authenticate");
}

void Bot::listenForMessages(void)
{
    while (true)
    {
        int poll_count = poll(&_bot_pollfd, 1, -1);
        if (poll_count == -1)
            throw std::runtime_error("Poll error");

        if (_bot_pollfd.revents & POLLIN)
            handleMessage();
    }
}

void Bot::handleMessage(void)
{
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytes = read(_bot_fd, buffer, sizeof(buffer) - 1);
    if (bytes <= 0)
        throw std::runtime_error("Server connection closed");

    processCommand(std::string(buffer));
}

void Bot::processCommand(const std::string &command)
{
    std::string sender = command.substr(1, command.find('!') - 1);
    std::string request = command.substr(command.rfind(':') + 1);
    request.erase(request.find_last_not_of(" \r\n") + 1);
    std::string message = _help_map[request];
    if (message.empty() == true)
        message = "!join | !mode | !part | !kick | !topic | !invite | !privmsg | !die";

    std::string reply = "PRIVMSG " + sender + " :" + message + "\r\n";
    if (send(_bot_fd, reply.c_str(), reply.length(), 0) < 0)
        throw std::runtime_error("Failed to send response");
}
