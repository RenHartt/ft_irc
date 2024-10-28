#include <Client.hpp>
#include <Server.hpp>
#include <Utils.hpp>

Client::Client(int client_fd)
    : _client_fd(client_fd),
      _nickname(itoa(client_fd))
{}

int Client::getFd() const { return (this->_client_fd); }

bool Client::getIsRegistered() { return _isRegistered; }
const std::string &Client::getNickname() const { return _nickname; }
const std::string &Client::getUsername() const { return _username; }
const std::string &Client::getRealname() const { return _realname; }

void Client::setIsRegistered(bool isRegistered) { _isRegistered = isRegistered; }
void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
void Client::setUsername(const std::string &username) { _username = username; }
void Client::setRealname(const std::string &realname) { _realname = realname; }
bool Client::isOperator() const {

	//TODO clients right for operator !!!!!
    return false; 
}
