#pragma once

#include <Channel.hpp>
#include <Utils.hpp>

#include <string>

class Channel;

class Client
{
  public:
    Client(int fd);

    bool               getIsRegistered();
    const std::string &getNickname() const;
    const std::string &getUsername() const;
    const std::string &getRealname() const;
    int                getFd() const;

    void setIsRegistered(bool isRegistered);
    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setRealname(const std::string &realname);

  private:
    int         _client_fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    bool        _isRegistered;

    ClientMap _channels;
};
