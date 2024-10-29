#pragma once

#include "BotPlugin.hpp"
#include "BotPluginRegistry.hpp"
#include "Client.hpp"
#include <sys/socket.h>
#include <vector>

class BotPING : public BotPlugin
{
  public:
    void execute(Client *sender, std::vector<std::string>)
    {
        std::string test("pong");
        send(sender->getFd(), test.c_str(), test.size(), 0);
    }
};

REGISTER_BOT("PING", BotPING)
