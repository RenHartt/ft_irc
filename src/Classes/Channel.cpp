#include "Utils.hpp"
#include <Server.hpp>
#include <cstring>

Channel::Channel(const std::string &channel_name)
	: _channel_name(channel_name), 
	_password("")
{
    memset(&channel_settings, 0, sizeof(channel_settings));
}

Channel::Channel(const std::string &channel_name, const std::string &password)
    : _channel_name(channel_name),
      _password(password)
{
    memset(&channel_settings, 0, sizeof(channel_settings));
    channel_settings.k_enableKey = true;
}

std::string Channel::getChannelName(void) const { return _channel_name; }
std::string Channel::getPassword(void) const { return _password; }
std::string Channel::getTopic(void) const { return _topic.empty() ? "No topic set" : _topic; }
int Channel::getNbClient(void) const { return clients_rights.size(); }

void Channel::setTopic(const std::string &new_topic) { _topic = new_topic; }
void Channel::setPassword(const std::string &password) { _password = password; }

void Channel::addClient(Client *client, bool isOperator) { clients_rights[client->getFd()] = isOperator; }
void Channel::delClient(Client *client) { clients_rights.erase(client->getFd()); }

bool Channel::isMember(Client *client) { return clients_rights.find(client->getFd()) != clients_rights.end(); }
bool Channel::isOperator(Client *client) { return clients_rights.find(client->getFd())->second; }

void Channel::broadcastMessage(const std::string &message, Client *sender)
{
    for (OperatorMap::iterator it = clients_rights.begin(); it != clients_rights.end(); it++)
    {
        if (it->first != sender->getFd())
            send(it->first, message.c_str(), message.length(), 0);
    }
}
