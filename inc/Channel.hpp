#pragma once

#include <Client.hpp>

#include <iostream>
#include <map>
#include <stdint.h>

struct Rights {
    uint8_t write : 1;
    /* uint8_t     uploadImg : 1; */

    struct {
        uint8_t mute : 1;
        uint8_t kick : 1;
        uint8_t ban : 1;
        uint8_t invite : 1;
        uint8_t setPassWord : 1;
        uint8_t changeChannelVisibility : 1;
        uint8_t changeChannelName : 1;
        uint8_t changeGrantable : 1;
    } admin;
};

struct ClientRight {
    Rights rights;
    Rights grantable;
};

struct ChannelSettings {
    uint8_t inviteOnly : 1;
    uint8_t enableKey : 1;
    uint8_t hiddenMode : 1;
};

// need to be rework parse the input for better error message

inline void err_message(const char *str) { std::cerr << str << std::endl; }

#define SET_PERMISSION(grantor_rights, target_user, permission, state)                             \
    {                                                                                              \
        if (grantor_rights.grantable.permission)                                                   \
            target_user.rights.permission = state;                                                 \
        else                                                                                       \
            err_message("rights." #permission);                                                    \
    }

#define SET_PERMISSION_GRANTABLE(grantor_rights, target_user, permission, state)                   \
    {                                                                                              \
        if (grantor_rights.grantable.admin.changeGrantable && grantor_rights.grantable.permission) \
            target_user.grantable.permission = state;                                              \
        else                                                                                       \
            err_message("grantable." #permission);                                                 \
    }

class Client;

class Channel
{
  public:
    Channel(const std::string &channel_name);
    Channel(const std::string &channel_name, const std::string &password);

    void addClient(Client *client);
	void delCLient(Client *client);
    void broadcastMessage(const std::string &message, Client *sender);

    std::string getChannelName(void) const;
    std::string getPassword(void) const;
    std::string list_channel(Channel *channel) const;
    std::string getTopic() const;
    void        setTopic(const std::string &new_topic);
    bool        isMember(Client *client);

  private:
    std::string _channel_name;
    std::string _password;
    std::string _topic;

    std::map<int, ClientRight> _clients_rights;
    ChannelSettings            _channel_settings;
};
