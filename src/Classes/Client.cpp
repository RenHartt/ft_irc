#include <Client.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <sstream>

Client::Client(int client_fd) :
	_client_fd(client_fd)
{
	_isAuthenticated = false;
    _isRegistered = false;
}

int                Client::getFd() const { return (this->_client_fd); }
const std::string &Client::getNickname() const { return _nickname; }
const std::string &Client::getUsername() const { return _username; }
const std::string &Client::getBuffer() const { return _buffer; }
bool               Client::getIsAuthenticated() const { return _isAuthenticated; }
bool               Client::getIsRegistered() const { return _isRegistered; }

void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
void Client::setUsername(const std::string &username) { _username = username; }
void Client::setBuffer(const std::string &buffer) { _buffer = buffer; }
void Client::setIsAuthenticated(bool authenticated) { _isAuthenticated = authenticated; }
void Client::setIsRegistered(bool isRegistered) { _isRegistered = isRegistered; }

void Client::appendToBuffer(const std::string &buffer) { _buffer += buffer; }
void Client::clearBufferUpTo(std::size_t start, std::size_t end) { _buffer.erase(start, end + 1); }

std::vector<std::string> Client::splitCommand(const std::string &buffer)
{
    std::vector<std::string> splited;
    std::string              command(buffer);
    std::size_t              colonPos = command.find(':');
    std::string              trailing;

    if (colonPos != std::string::npos)
    {
        trailing = command.substr(colonPos + 1);
        command = command.substr(0, colonPos);
    }

    std::stringstream ss(command);
    std::string       token;

    while (ss >> token)
        splited.push_back(token);

    if (!trailing.empty())
        splited.push_back(trailing);

    return splited;
}

void Client::accumulateAndExtractCommand(const std::string &buffer, std::vector<std::string> &command)
{
    _buffer += buffer;
    size_t pos = _buffer.find('\n');
    if (pos != std::string::npos)
    {
        std::string commandLine = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 1);
        command = splitCommand(commandLine);
    }
}
