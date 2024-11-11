#pragma once

#include <Channel.hpp>
#include <Client.hpp>
#include <Command.hpp>
#include <Utils.hpp>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <poll.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

extern bool running;

class Client;
class Channel;
class Command;

class Server
{
  public:
    Server(const std::string &port, const std::string &password);
    ~Server(void);

    void run();
    void handleEvents();
    void acceptNewClient();
    void handleCommand(int client_fd);
    void broadcastServer(const std::string &message);

    int     getClientCount() const;
    int     getClientFdByNickname(const std::string &nickname) const;
    Client *getClientByNickname(const std::string &nickname) const;

    Channel *getChannelByChannelname(const std::string &channelname);

    ChannelMap getChannelsList(void) const;
    ClientMap  getClientsList(void) const;

    std::string         getName() const;
    std::vector<pollfd> getPollFds(void) const;
    std::string         getPassword() const;
    std::string         getCreationDate() const;

    void checkAuth(Client *client, std::string command) const;
    bool checkPassword(const std::string &password) const;
    bool NicknameAlreadyUsed(const std::string &nickname) const;

    void addChannel(const std::string &channel_name, Channel *channel);
    void addClient(int fd, Client *client);
    void delClient(int fd);

  private:
    int                 _server_fd;
    const int           _port;
    std::string         _password;
    std::string         _server_name;
    std::vector<pollfd> _poll_fds;
    uint8_t             _hash[32];

    Command     _command;
    ClientMap   _clients_list;
    ChannelMap  _channels_list;
    std::string _creationDate;

    void _initSockAddr(sockaddr_in &address);
    void _newFdToPoll(void);

    void _createSocket(void);
    void _bindSocket(void);
    void _listenSocket(void);
};
