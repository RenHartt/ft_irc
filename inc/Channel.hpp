#pragma once

#include <Client.hpp>
#include <Permission.hpp>
#include <map>
#include <stdint.h>

struct ChannelSettings {
    uint8_t inviteOnly : 1;
    uint8_t enableKey : 1;
    uint8_t hiddenMode : 1;
    uint8_t userLimit : 1;
};

// need to be rework parse the input for better error message

#define SET_PERMISSION_FUNC(permission)                                                            \
    void set_permission_##permission(const ClientRight &grantor, ClientRight &target, bool state)  \
    {                                                                                              \
        if (grantor.grantable.permission)                                                          \
            target.rights.permission = state;                                                      \
        else                                                                                       \
            err_message("rights." #permission);                                                    \
    }

#define SET_PERMISSION_ADMIN_FUNC(permission)                                                      \
    void set_permission_##permission(const ClientRight &grantor, ClientRight &target, bool state)  \
    {                                                                                              \
        if (grantor.grantable.admin.permission)                                                    \
            target.rights.admin.permission = state;                                                \
        else                                                                                       \
            err_message("rights." #permission);                                                    \
    }

#define SET_PERMISSION_SETMAP(value) SET_MAP[#value] = set_permission_##value;

class Client;

class Channel
{
  public:
    Channel(const std::string &channel_name);
    Channel(const std::string &channel_name, const std::string &password);

    void addClient(Client *client);
    void delClient(Client *client);
    void broadcastMessage(const std::string &message, Client *sender);

    std::string   getChannelName(void) const;
    std::string   getPassword(void) const;
    std::string   getTopic(void) const;
    SetPermission getSetPermission(const std::string &mode);

    void setTopic(const std::string &new_topic);

    std::string list_channel(Channel *channel) const;
    bool        isMember(Client *client);

  private:
    std::string _channel_name;
    std::string _password;
    std::string _topic;

    std::map<int, ClientRight> _clients_rights;
    ChannelSettings            _channel_settings;

    static MapSetPermission _clients_set_permission_func;
    static MapSetPermission _set_map_permission(void);
};
