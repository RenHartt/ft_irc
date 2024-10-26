#pragma once

#include <Client.hpp>
#include <string>
#include <map>

std::string itoa(int value);

std::vector<std::string> split(const std::string &str, char delimiter);

int getFdByNickname(const std::string &nickname, std::map<int, Client *>clients_list);
