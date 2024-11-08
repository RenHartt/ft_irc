#include "Utils.hpp"
#include <Server.hpp>
#include <cstring>

Channel::Channel(const std::string &channel_name)
	: _channel_name(channel_name)
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
int Channel::getNbClient(void) const { return clients.size(); }

void Channel::setTopic(const std::string &new_topic) { _topic = new_topic; }
void Channel::setPassword(const std::string &password) { _password = password; }

void Channel::addClient(Client *client) { clients[client->getFd()] = client; }
void Channel::delClient(Client *client) { clients.erase(client->getFd()); }

void Channel::addOperator(Client *client) { operators[client->getFd()] = client; }
void Channel::delOperator(Client *client) { operators.erase(client->getFd()); }

bool Channel::isMember(Client *client) { return clients.find(client->getFd()) != clients.end(); }
bool Channel::isOperator(Client *client) { return operators.find(client->getFd())->second; }

void Channel::broadcastMessage(const std::string &message, Client *client)
{
    for (ClientMap::iterator it = clients.begin(); it != clients.end(); it++)
    {
		if (client == NULL || it->second != client)
			send(it->first, message.c_str(), message.length(), 0);
    }
}
