#pragma once 

#include <string>
#include <vector>
#include <Client.hpp>

class BotPlugin
{
  public:
    virtual ~BotPlugin();
    virtual void execute(Client *sender, std::vector<std::string> command) = 0;
};
