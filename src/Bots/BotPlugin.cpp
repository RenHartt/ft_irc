#include "bots/BotPlugin.hpp"
#include "Server.hpp"
#include <iostream>

BotPlugin::BotPlugin(const std::string &bot_name) : _bot_name(bot_name)
{
    std::cout << _bot_name << std::endl;
}

static void sendMsg(int fd, const std::string &str) { send(fd, str.c_str(), str.length(), 0); }

void BotPlugin::connect(const std::string &port, const std::string &password)
{
    _network_manager.setPort(port);
    _network_manager.createSocket();
	_network_manager.connect_serv();

    int fd = _network_manager.getFd();
    sendMsg(fd, "NICK " + _bot_name);
    sendMsg(fd, "PASS " + password);
}
