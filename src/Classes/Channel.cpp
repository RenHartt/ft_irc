#include "Channel.hpp"

Channel::Channel(const std::string &name) : _name(name) {}

Channel::~Channel() {}

const std::string &Channel::getName() const { return _name; }

const std::vector<Client *> &Channel::getClients() const { return _clients; }
