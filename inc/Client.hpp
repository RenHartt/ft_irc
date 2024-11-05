#pragma once

#include <Utils.hpp>

#include <string>

class Channel;

class Client
{
  public:
    Client(int fd);

    const std::string &getBuffer() const;
    const std::string &getNickname() const;
    const std::string &getUsername() const;
    const std::string &getRealname() const;
    bool               getAuthenticated() const;
    bool               getIsRegistered();
    int                getFd() const;

    void setBuffer(const std::string &buffer);
    void setIsRegistered(bool isRegistered);
    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setRealname(const std::string &realname);
    void setAuthenticated(bool authenticated);

    void appendToBuffer(const std::string &buffer);
    void clearBufferUpTo(std::size_t start, std::size_t end);

    std::vector<std::string> splitCommand(const std::string &buffer);
    void accumulateAndExtractCommand(const std::string &buffer, std::vector<std::string> &command);

  private:
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
    int         _client_fd;
    bool        _isRegistered;
    bool        _authenticated;

    std::string _buffer;

    ClientMap _channels;
};
