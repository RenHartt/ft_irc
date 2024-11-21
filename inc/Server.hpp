#pragma once

#include <cstdlib>
#include <cstring>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <Client.hpp>
#include <Channel.hpp>
#include <Command.hpp>

extern bool running;

class Client;
class Channel;
class Command;

class Server
{
  public:
    Server(const std::string &port, const std::string &password);
    ~Server(void);

	int					getFd() const;
	int					getPort() const;
    int                 getClientCount() const;
    int                 getClientFdByNickname(const std::string &nickname) const;
    Client             *getClientByNickname(const std::string &nickname) const;
    Channel            *getChannelByChannelname(const std::string &channelname);
    ClientMap           getClientsList(void) const;
    ChannelMap          getChannelsList(void) const;
    std::string         getCreationDate() const;
    std::string         getName() const;
    std::vector<pollfd> getPollFds(void) const;

    void checkAuth(Client *client, const std::string &command) const;
    bool NicknameAlreadyUsed(const std::string &nickname) const;

    void addChannel(const std::string &channel_name, Channel *channel);
    void addClient(int fd, Client *client);

    void delClient(int fd);

    void run();
    void handleEvents();
    void handleCommand(int client_fd);
    void acceptNewClient();
    void broadcastServer(const std::string &message);

    std::string getPassword() const;
    bool        checkPassword(const std::string &password) const;

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
