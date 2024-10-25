#pragma once

#include <map>
#include <string>
#include <vector>

class Server;
class Client;
class Channel;
class Command;

typedef void (Command::*CmdAddr)(Client *, std::vector<std::string>);
typedef std::map<std::string, CmdAddr> CommandMap;

class Command
{
  public:
    Command(Server *server);
    void exec(const std::string &cmd, Client *client, std::vector<std::string>);

  private:
    static CommandMap _commands;
    Server           *_server;

    static CommandMap _initCommands();
    CmdAddr           _find(const std::string &cmd);

    void _createChannel(Client *client, const std::string &channel_name);
    void _joinChannel(Client *client, Channel *channel);

    void _executeJoin(Client *client, std::vector<std::string>);
    void _executeUser(Client *client, std::vector<std::string>);
    void _executeNick(Client *client, std::vector<std::string>);
    void _executePart(Client *client, std::vector<std::string>);
    void _executePrivmsg(Client *client, std::vector<std::string>);
    void _executeQuit(Client *client, std::vector<std::string>);
    void _executeHelp(Client *client, std::vector<std::string>);
    void _executeWhoami(Client *client, std::vector<std::string> args);

    void _executeStop(Client *client, std::vector<std::string>);
};
