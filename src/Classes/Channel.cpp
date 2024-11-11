#include "Utils.hpp"
#include <Server.hpp>
#include <cstring>

Channel::Channel(const std::string &channel_name) : _channel_name(channel_name), _password("")
{
    memset(&_channel_settings, 0, sizeof(_channel_settings));
}

Channel::Channel(const std::string &channel_name, const std::string &password)
    : _channel_name(channel_name),
      _password(password)
{
    memset(&_channel_settings, 0, sizeof(_channel_settings));
    _channel_settings.k_enableKey = true;
}

const std::string     Channel::getChannelName(void) const { return _channel_name; }
const std::string     Channel::getPassword(void) const { return _password; }
const std::string     Channel::getTopic(void) const { return _topic.empty() ? "No topic set" : _topic; }
const std::string     Channel::getTopicSetter() const { return _topicSetter; }
int             Channel::getNbClient(void) const { return _clients.size(); }
ClientMap       Channel::getClientsMap(void) const { return _clients; }
ClientMap       Channel::getOperatorsMap(void) const { return _operators; }
ClientMap       Channel::getGuestsMap(void) const { return _guests; }
ChannelSettings Channel::getChannelSettings(void) const { return _channel_settings; }

void Channel::setTopic(const std::string &new_topic) { _topic = new_topic; }
void Channel::setTopicSetter(const std::string &setter) { _topicSetter = setter; }
void Channel::setPassword(const std::string &password) { _password = password; }
void Channel::setChannelSettings(ChannelSettings &channel_settings) { _channel_settings = channel_settings; }

void Channel::addGuest(Client *client) { _guests[client->getFd()] = client; }
void Channel::addClient(Client *client) { _clients[client->getFd()] = client; }
void Channel::addOperator(Client *client) { _operators[client->getFd()] = client; }

void Channel::delGuest(Client *client) { _guests.erase(client->getFd()); }
void Channel::delClient(Client *client) { _clients.erase(client->getFd()); }
void Channel::delOperator(Client *client) { _operators.erase(client->getFd()); }

bool Channel::isMember(Client *client) { return _clients[client->getFd()]; }
bool Channel::isOperator(Client *client) { return _operators[client->getFd()]; }
bool Channel::isGuest(Client *client) { return _guests[client->getFd()]; }

void Channel::broadcastMessage(const std::string &message, Client *client)
{
    for (ClientMap::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (client == NULL || it->second != client)
            send(it->first, message.c_str(), message.length(), 0);
    }
}
