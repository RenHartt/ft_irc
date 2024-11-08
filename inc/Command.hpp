#pragma once

#include <Utils.hpp>
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

    void exec(const std::string &cmd, Client *client, std::vector<std::string> args);

  private:
    static CommandMap _commands;
    Server           *_server;

    static CommandMap _initCommands();
    CmdAddr           _find(const std::string &cmd);

	void _executeKick(Client *client, std::vector<std::string> args);
    void _executeJoin(Client *client, std::vector<std::string> args);
    void _executeUser(Client *client, std::vector<std::string> args);
    void _executeNick(Client *client, std::vector<std::string> args);
    void _executePart(Client *client, std::vector<std::string> args);
    void _executePrivmsg(Client *client, std::vector<std::string> args);
    void _executeHelp(Client *client, std::vector<std::string> args);
    void _executeTopic(Client *, std::vector<std::string> args);
    void _executeDie(Client *client, std::vector<std::string> args);
	void _executeInvite(Client *client, std::vector<std::string> args);
	void _executePass(Client *client, std::vector<std::string> args);
	void _executeMode(Client* client, std::vector<std::string> args);
};

std::string getListOfModes(Channel *channel);
