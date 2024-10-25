#pragma once

#include <Client.hpp>
#include <string>
#include <map>

std::string itoa(int value);

int getFdByNickname(const std::string &nickname, std::map<int, Client *>clients_list);
