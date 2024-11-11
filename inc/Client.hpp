#pragma once

#include <string>
#include <Utils.hpp>

class Client
{
  public:
    Client(int fd);

    int getFd() const;

    const std::string &getNickname() const;
    const std::string &getUsername() const;
    const std::string &getBuffer() const;
    bool               getIsAuthenticated() const;
    bool               getIsRegistered() const;

    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setBuffer(const std::string &buffer);
    void setIsAuthenticated(bool authenticated);
    void setIsRegistered(bool isRegistered);

    void appendToBuffer(const std::string &buffer);
    void clearBufferUpTo(std::size_t start, std::size_t end);

    std::vector<std::string> splitCommand(const std::string &buffer);
    void accumulateAndExtractCommand(const std::string &buffer, std::vector<std::string> &command);

  private:
    int         _client_fd;
    std::string _nickname;
    std::string _username;
    std::string _buffer;
    bool        _isAuthenticated;
    bool        _isRegistered;

    ClientMap _channels;
};
