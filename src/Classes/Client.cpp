#include <Client.hpp>
#include <Server.hpp>
#include <Utils.hpp>

Client::Client(int client_fd)
    : _client_fd(client_fd),
      _nickname(itoa(client_fd))
{}

int Client::getFd() { return (this->_client_fd); }

std::string &Client::getNickname() { return _nickname; }
std::string &Client::getUsername() { return _username; }
std::string &Client::getRealname() { return _realname; }

void Client::setNickname(const std::string &nickname) { _nickname = nickname; }

void Client::setUsername(const std::string &username) { _username = username; }
void Client::setRealname(const std::string &realname) { _realname = realname; }
