#pragma once

#include "NetworkManager.hpp"
#include <Client.hpp>
#include <string>
#include <vector>

class BotPlugin
{
  public:
    BotPlugin(const std::string &bot_name);

    virtual ~BotPlugin();

    virtual void execute(Client *sender, std::vector<std::string> command) = 0;
    void         connect(const std::string &port, const std::string &passwod);

  private:
    NetworkManager _network_manager;
    std::string    _bot_name;
};
