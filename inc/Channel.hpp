#pragma once

#include <Client.hpp>

#include <map>

class Client;

class Channel
{
  public:
    Channel();

  private:
    std::map<int, Client *> _clients_list;
};
