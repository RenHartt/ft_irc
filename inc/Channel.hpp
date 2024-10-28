#pragma once

#include <Client.hpp>

#include <Permission.hpp>
#include <map>
#include <stdint.h>

struct ChannelSettings {
    uint8_t inviteOnly : 1;
    uint8_t enableKey : 1;
    uint8_t hiddenMode : 1;
};

class Client;

class Channel
{
  public:
    Channel(const std::string &channel_name);
    Channel(const std::string &channel_name, const std::string &password);

    void addClient(Client *client);
    void broadcastMessage(const std::string &message, Client *sender);

    std::string getChannelName(void) const;
    std::string getPassword(void) const;
    std::string list_channel(Channel *channel) const;

    bool isMember(Client *client);

  private:
    std::string _channel_name;
    std::string _password;
    std::string _topic;

    std::map<int, ClientRight> _clients_rights;
    ChannelSettings            _channel_settings;

    static MapSetPermission _clients_set_permission_func;
    static MapSetPermission _set_map_permission(void);
};
