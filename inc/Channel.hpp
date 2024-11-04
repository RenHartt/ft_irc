#pragma once

#include <Client.hpp>
#include <Permission.hpp>
#include <map>
#include <stdint.h>

// need to be formatted with the letter that correspond first
// example : uint8_t f_foo() -> this will set the value of foo when f is send
struct ChannelSettings {
    uint8_t i_inviteOnly : 1;
    uint8_t t_topicRestriction : 1;
    uint8_t k_enableKey : 1;
    uint8_t l_userLimit;
};

class Client;

class Channel
{
  public:
    Channel(const std::string &channel_name);
    Channel(const std::string &channel_name, const std::string &password);

    void addClient(Client *client, bool isOperator);
    void delClient(Client *client);
    void broadcastMessage(const std::string &message, Client *sender);

    std::string   getChannelName(void) const;
    std::string   getPassword(void) const;
    std::string   getTopic(void) const;
    SetPermission getSetPermission(const std::string &mode);

    void setTopic(const std::string &new_topic);
    void setPassword(const std::string &password);

    std::string list_channel(Channel *channel) const;
    bool        isMember(Client *client);
    bool        isOperator(Client *client);

    ChannelSettings            channel_settings;
    std::map<int, ClientRight> _clients_rights;

  private:
    std::string _channel_name;
    std::string _password;
    std::string _topic;

    static MapSetPermission _clients_set_permission_func;
    static MapSetPermission _set_map_permission(void);
};
