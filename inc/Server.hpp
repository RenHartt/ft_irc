#pragma once

#include <Channel.hpp>
#include <Client.hpp>
#include <Command.hpp>
#include <sys/ptrace.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "sha256.h"
#include "Client.hpp"
#include "Command.hpp"
#include "Utils.hpp"

extern bool running;

class Server
{
  public:
    Server(const std::string &port, const std::string &password);
    ~Server(void);

    void                run();
    void                handleEvents();
    void                acceptNewClient();
    void                handleCommand(int client_fd);
    void                updateNickname(int client_fd, const std::string &new_nickname);
    std::string         getName() const;
    int                 getClientCount() const;
    bool                NicknameAlreadyUsed(const std::string &nickname);
    int                 getFdByNickname(const std::string &nickname);
    Client             *getClientbyNickname(const std::string &nickname);
    ChannelMap          getChannelsList(void) const;
    ClientMap           getClientsList(void) const;
    std::vector<pollfd> getPollFds(void) const;
    std::string         getPassword();
	bool				checkPassword(const std::string &password) const;

    void addChannel(const std::string &channel_name, Channel *channel);
    void addClient(int fd, Client *client);
    void delClient(int fd, Client *client);
    void removeClient(int fd);

    std::vector<std::string> splitCommand(const std::string &buffer);

  private:
    int                 _server_fd;
    const int           _port;
    std::string         _password;
    std::string         _server_name;
    std::vector<pollfd> _poll_fds;
	uint8_t				_hash[32];

    ClientMap  _clients_list;
    ChannelMap _channels_list;

    void _initSockAddr(sockaddr_in &address);
    void _newFdToPoll(void);

    void _createSocket(void);
    void _bindSocket(void);
    void _listenSocket(void);

    Command _command;
};
