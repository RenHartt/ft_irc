#pragma once

#include <map>
#include <string>
#include <vector>

struct ClientRight;
class Client;
class Channel;

typedef std::map<int, Client *>                          ClientMap;
typedef std::map<std::string, Channel *>                 ChannelMap;
typedef std::vector<std::pair<std::string, std::string> >ChannelPasswordList;
typedef std::map<int, ClientRight>                       ClientRightMap;

std::string itoa(int value);

std::vector<std::string> split(const std::string &str, char delimiter);

int getFdByNickname(const std::string &nickname, ClientMap clients_list);

bool isValidChannelName(const std::string &channel_name);
