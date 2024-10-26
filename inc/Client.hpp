#pragma once

#include <Utils.hpp>
#include <Channel.hpp>

#include <queue>
#include <string>

class Channel;

class Client
{
  public:
    Client(int fd);

    int          getFd();
    std::string &getNickname();
    std::string &getUsername();
    std::string &getRealname();

    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setRealname(const std::string &realname);

  private:
    int         _client_fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;

    ClientMap _channels;
    std::queue<std::string> _private_message;
};
