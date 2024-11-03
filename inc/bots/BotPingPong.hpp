#pragma once

#include "BotPlugin.hpp"
#include "BotPluginRegistry.hpp"
#include "Client.hpp"
#include <iostream>
#include <sys/socket.h>
#include <vector>

class BotPING : public BotPlugin
{
  public:
	  // because of cpp 98 need to be rm
	BotPING(const std::string &name) : BotPlugin(name) { std::cout << "PING" << std::endl;}
    void execute(Client *sender, std::vector<std::string>)
    {
        std::string test("pong");
        send(sender->getFd(), test.c_str(), test.size(), 0);
    }
};

