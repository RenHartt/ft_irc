#pragma once

#include <map>
#include <string>
#include <vector>

class Client;
class Channel;
class Command;

typedef std::map<int, Client *>          ClientMap;
typedef std::map<std::string, Channel *> ChannelMap;
typedef std::vector<std::pair<std::string, std::string> > ChannelPasswordList;
typedef void (Command::*CmdAddr)(Client *, std::vector<std::string> &);
typedef std::map<std::string, CmdAddr> CommandMap;

std::string itoa(int value);
std::vector<std::string> split(const std::string &str, char delimiter);

bool isValidNickname(const std::string &nickname);
bool isValidChannelName(const std::string &channel_name);

void handleSignal(int signal);
