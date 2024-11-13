#pragma once

#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>

typedef std::map<std::string, std::string> HelpMap;

class Bot
{
  public:
    Bot(const std::string &server_address, int port, const std::string &password);
    ~Bot(void);

    HelpMap getMap(void) const;
    void    setMap(const HelpMap &help_map);

	void initHelpMap(void);

    void connectToServer(void);
    void authenticate(void);

    void listenForMessages(void);
    void handleMessage(void);
    void processCommand(const std::string &command);

  private:
    int           _bot_fd;
    std::string   _server_address;
    int           _port;
    std::string   _password;
    struct pollfd _bot_pollfd;
    HelpMap       _help_map;
};
