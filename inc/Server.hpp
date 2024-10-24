#pragma once

#include <Channel.hpp>
#include <Client.hpp>
#include <Command.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>

class Client;
class Channel;

class Server
{
  public:
    Server(std::string port, std::string password);

    std::vector<std::string> splitCommand(const char *buffer);

    void run();
    void handleEvents();
    void acceptNewClient();
    void handleCommand(int client_fd);
	void updateNickname(int client_fd, const std::string &new_nickname);

    std::map<std::string, Client *> getClientsList(void) const;
    std::vector<pollfd>             getPollFds(void) const;

  private:
    int                 _server_fd;
    const int           _port;
    std::string         _password;
    std::string         _server_name;
    std::vector<pollfd> _poll_fds;

    std::map<std::string, Client *>  _clients_list;
    std::map<std::string, Channel *> _channels_list;

    Command _command;
};

std::string itoa(int value);
