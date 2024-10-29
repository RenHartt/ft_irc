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
std::string Channel::list_channel(Channel *) const
{
    return _topic.empty() ? "No topic set" : _topic;
}
void Channel::addClient(Client *client)
{
    ClientRight defaultRight;

    _clients_rights[client->getFd()] = defaultRight;
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

std::string Channel::getTopic() const { return _topic; }

void Channel::setTopic(const std::string &new_topic) { _topic = new_topic; }
