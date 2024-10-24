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
    } admin;
};

struct ClientRight {
    Rights rights;
    Rights grantable;
};

// need to be rework parse the input for better error message
void err_message(const char *str) { std::cerr << str << std::endl; }

#define SET_PERMISSION(grantor_rights, target_user, permission, state)         \
    {                                                                          \
        if (grantor_rights.grantable.permission)                               \
            target_user.rights.permission = state;                             \
        else                                                                   \
            err_message(#permission);                                          \
    }

class Client;

class Channel
{
  public:
    Channel();

  private:
    std::map<int, ClientRight> _clients_rights;
};
