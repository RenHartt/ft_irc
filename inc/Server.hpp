#pragma once

#include <Channel.hpp>
#include <Client.hpp>
#include <Command.hpp>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <poll.h>
#include <sys/ptrace.h>
#include <sys/socket.h>

extern bool running;

class Client;
class Channel;
class Command;

#ifndef SERVER_SIZE
#    define SERVER_SIZE 3
#endif

class Server
{
  public:
    Server(const std::string &port, const std::string &password);
    ~Server(void);

    std::string getServerName() const;
    std::string getPassword() const;
    int         getPort() const;
    int         getClientCount() const;

    int      getClientFdByNickname(const std::string &nickname) const;
    Client  *getClientByNickname(const std::string &nickname) const;
    Channel *getChannelByChannelname(const std::string &channelname);

    ClientMap  getClientsList(void) const;
    ChannelMap getChannelsList(void) const;

    std::string         getCreationDate() const;
    std::vector<pollfd> getPollFds(void) const;

    void checkAuth(Client *client, const std::string &command) const;
    bool NicknameAlreadyUsed(const std::string &nickname) const;

    void addChannel(const std::string &channel_name, Channel *channel);
    void addClient(int fd, Client *client);

    void delClient(int fd);

    void run();
    void init();
    void handleEvents();
    void handleCommand(int client_fd);
    void acceptNewClient();
    void broadcastServer(const std::string &message);

    bool checkPassword(const std::string &password) const;

  private:
    int                 _server_fd;
    const int           _port;
    std::string         _password;
    std::string         _server_name;
    std::string         _creationDate;
    std::vector<pollfd> _poll_fds;
    uint8_t             _hash[32];

    Command    _command;
    ClientMap  _clients_list;
    ChannelMap _channels_list;

    void _initSockAddr(sockaddr_in &address);
    void _newFdToPoll(void);

    void _createSocket(void);
    void _bindSocket(void);
    void _listenSocket(void);
};
