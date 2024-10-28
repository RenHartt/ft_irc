#include "Utils.hpp"
#include <Channel.hpp>
#include <Server.hpp>
#include <cstring>

Channel::Channel(const std::string &channel_name)
    : _channel_name(channel_name),
      _password("")
{
    memset(&_channel_settings, 0, sizeof(_channel_settings));
}

Channel::Channel(const std::string &channel_name, const std::string &password)
    : _channel_name(channel_name),
      _password(password)
{
    memset(&_channel_settings, 0, sizeof(_channel_settings));
}
/* getter */

std::string Channel::getChannelName(void) const { return _channel_name; }

std::string Channel::getPassword(void) const { return _password; }
std::string Channel::list_channel(Channel *) const {
    return _topic.empty() ? "No topic set" : _topic; 
}
void Channel::addClient(Client *client)
{
    ClientRight defaultRight;

    _clients_rights[client->getFd()] = defaultRight;
}

void Channel::broadcastMessage(const std::string &message, Client *sender)
{
    for (std::map<int, ClientRight>::iterator it = _clients_rights.begin();
         it != _clients_rights.end(); it++)
    {
        int fd = it->first;
        if (fd != sender->getFd())
            send(fd, message.c_str(), message.length(), 0);
    }
}

bool Channel::isMember(Client *client)
{
    int client_fd = client->getFd();
    
    return _clients_rights.find(client_fd) != _clients_rights.end();
}

std::string Channel::getTopic() const {
    return _topic;
}

void Channel::setTopic(const std::string &new_topic) {
    _topic = new_topic;
}


