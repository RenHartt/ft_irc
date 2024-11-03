#pragma once

#include <netinet/in.h>
#include <string>

#define PORT_NOT_SET -1

class NetworkManager
{
  public:
    NetworkManager(const std::string &port);
    NetworkManager(const int port);
    NetworkManager();
    ~NetworkManager();

    void createSocket();
    void bindSocket();
    void listenSocket();

    int getPort(void) const;
    int getFd(void) const;

    void setPort(const std::string &port);
    void setPort(const int port);

  private:
    void _initSockAddr(sockaddr_in &address);

    int _fd;
    int _port;
};
