#pragma once

#include <map>
#include <string>
#include <vector>

class Client;
class Channel;
struct ClientRight;

typedef std::map<int, Client *>                           ClientMap;
typedef std::map<std::string, Channel *>                  ChannelMap;
typedef std::vector<std::pair<std::string, std::string> > ChannelPasswordList;
typedef std::map<int, ClientRight>                        ClientRightMap;
typedef std::map<int, bool>                               OperatorMap;

std::string itoa(int value);

std::vector<std::string> split(const std::string &str, char delimiter);

int getFdByNickname(const std::string &nickname, ClientMap clients_list);

bool isValidChannelName(const std::string &channel_name);
bool isValidNickname(const std::string &nickname);
bool isValidUsername(const std::string &username);

void handleSignal(int signal);
