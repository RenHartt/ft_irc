#pragma once

#include <map>
#include <vector>
#include <string>

class Client;
class Channel;

typedef std::map<int, Client *> ClientMap;
typedef std::map<std::string, Channel *> ChannelMap;

#include <Client.hpp>

std::string itoa(int value);

std::vector<std::string> split(const std::string &str, char delimiter);

int getFdByNickname(const std::string &nickname, ClientMap clients_list);
