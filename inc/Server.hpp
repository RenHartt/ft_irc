#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include "NetworkManager.hpp"
#include "Utils.hpp"
#include "sha256.h"
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
#include <unistd.h>
#include <vector>

extern bool running;

class Server
{
  public:
    Server(const std::string &port, const std::string &password);
    ~Server();

    void run();
    void handleEvents();

    // -------------------------------
    // Client Management
    // -------------------------------

    void      acceptNewClient();
    int       getClientCount() const;
    int       getFdByNickname(const std::string &nickname);
    Client   *getClientByNickname(const std::string &nickname);
    ClientMap getClientsList() const;

    void addClient(int fd, Client *client);
    void delClient(int fd, Client *client);
    void removeClient(int fd);
    bool isNicknameAlreadyUsed(const std::string &nickname);
    void updateNickname(int client_fd, const std::string &new_nickname);

    // -------------------------------
    // Channel Management
    // -------------------------------

    void       addChannel(const std::string &channel_name, Channel *channel);
    ChannelMap getChannelsList() const;

    // -------------------------------
    // Command Handling
    // -------------------------------

    void                     handleCommand(int client_fd);
    std::vector<std::string> splitCommand(const char *buffer);

    // -------------------------------
    // Security and Authentication
    // -------------------------------

    std::string getPassword();
    bool        checkPassword(const std::string &password) const;

    // -------------------------------
    // Getters
    // -------------------------------

    std::string         getName() const;
    std::vector<pollfd> getPollFds() const;

  private:
    void _newFdToPoll();

    std::string _server_name;

	NetworkManager		_network_manager;
    std::vector<pollfd> _poll_fds;

    std::string _password; // need to be rm
    uint8_t     _hash[32];

    ClientMap  _clients_list;
    ChannelMap _channels_list;

    Command _command;
};
