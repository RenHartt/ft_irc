#pragma once

#include <map>
#include <string>
#include <vector>
#include <Utils.hpp>

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

    void _createChannel(Client *client, const std::string &channel_name,
                        const std::string &password);
    void _joinChannel(Client *client, Channel *channel);

	void sendToChannel(Client *sender, const std::string &recipient, const std::string &message, ChannelMap &channel_map);
	void sendToClient(Client *sender, const std::string &recipient, const std::string &message, ClientMap &clients_list);

	void routePrivmsg(Client *client, std::vector<std::string> args);

	void _executeKick(Client *client, std::vector<std::string> args);
    void _executeJoin(Client *client, std::vector<std::string> args);
    void _executeUser(Client *client, std::vector<std::string> args);
    void _executeNick(Client *client, std::vector<std::string> args);
    void _executePart(Client *client, std::vector<std::string> args);
	void _executePrivmsg(Client *client, std::vector<std::string> args);
    void _executeQuit(Client *client, std::vector<std::string> args);
    void _executeHelp(Client *client, std::vector<std::string> args);
    void _executeWhoami(Client *client, std::vector<std::string> args);
    void _executeList(Client *client, std::vector<std::string> args);
	void _executeTopic(Client *client, std::vector<std::string> args);
    void _executeStop(Client *client, std::vector<std::string> args);
};
