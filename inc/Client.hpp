#pragma once

#include <Utils.hpp>

#include <string>

class Channel;

class Client
{
  public:
    Client(int fd);

    int                getFd() const;

    const std::string &getBuffer() const;
    const std::string &getNickname() const;
    const std::string &getUsername() const;
    bool               getIsAuthenticated() const;
    bool               getIsRegistered() const;

    void setBuffer(const std::string &buffer);
    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setIsAuthenticated(bool authenticated);
    void setIsRegistered(bool isRegistered);

    void appendToBuffer(const std::string &buffer);
    void clearBufferUpTo(std::size_t start, std::size_t end);

    std::vector<std::string> splitCommand(const std::string &buffer);
    void accumulateAndExtractCommand(const std::string &buffer, std::vector<std::string> &command);

  private:
    std::string _nickname;
    std::string _username;
    std::string _hostname;
    int         _client_fd;
    bool        _isRegistered;
    bool        _isAuthenticated;

    std::string _buffer;

    ClientMap _channels;
};
