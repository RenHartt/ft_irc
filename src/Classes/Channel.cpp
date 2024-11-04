#include "Utils.hpp"
#include <Server.hpp>
#include <cstring>

MapSetPermission Channel::_clients_set_permission_func = Channel::_set_map_permission();

#define SET_PERMISSION SET_PERMISSION_FUNC
#define SET_PERMISSION_ADMIN SET_PERMISSION_ADMIN_FUNC
#include <PermissionList.hpp>

#define SET_MAP _set_map // use by the SET_PERMISSION_MAP to now how the map is call
#define SET_PERMISSION SET_PERMISSION_SETMAP
#define SET_PERMISSION_ADMIN SET_PERMISSION_SETMAP

MapSetPermission Channel::_set_map_permission(void)
{
    MapSetPermission SET_MAP;
#include <PermissionList.hpp>
    return (SET_MAP);
}

Channel::Channel(const std::string &channel_name) : _channel_name(channel_name), _password("")
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
/* getter */

std::string Channel::getChannelName(void) const { return _channel_name; }

std::string Channel::getPassword(void) const { return _password; }

SetPermission Channel::getSetPermission(const std::string &mode)
{
	return _clients_set_permission_func[mode];
}

std::string Channel::list_channel(Channel *) const
{
    return _topic.empty() ? "No topic set" : _topic;
}

void Channel::addClient(Client *client, bool isOperator)
{
    ClientRight right;
	right.rights.isOperator = isOperator;

    _clients_rights[client->getFd()] = right;
}

void Channel::delClient(Client *client)
{
	_clients_rights.erase(client->getFd());
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

bool Channel::isOperator(Client *client)
{
    int client_fd = client->getFd();

	ClientRightMap::iterator it = _clients_rights.find(client_fd);

	return it->second.rights.isOperator;
}

std::string Channel::getTopic() const { return _topic; }

void Channel::setTopic(const std::string &new_topic) { _topic = new_topic; }

void Channel::setPassword(const std::string &password) { _password = password; }

int Channel::getNbClient(void) const { return _clients_rights.size(); }
