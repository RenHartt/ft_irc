#pragma once

#include <Client.hpp>
#include <Utils.hpp>
#include <stdint.h>

struct ChannelSettings {
    uint8_t i_inviteOnly : 1;
    uint8_t t_topicRestriction : 1;
    uint8_t k_enableKey : 1;
    uint8_t l_userLimit;
};

class Channel
{
  public:
    Channel(const std::string &channel_name);
    Channel(const std::string &channel_name, const std::string &password);

    std::string     getChannelName(void) const;
    std::string     getPassword(void) const;
    std::string     getTopic(void) const;
    std::string     getTopicSetter() const;
    int             getNbClient(void) const;
    ClientMap       getClientsMap(void) const;
    ClientMap       getOperatorsMap(void) const;
    ClientMap       getGuestsMap(void) const;
    ChannelSettings getChannelSettings(void) const;

    void setPassword(const std::string &password);
    void setTopic(const std::string &new_topic);
    void setTopicSetter(const std::string &topicSetter);
    void setChannelSettings(ChannelSettings channel_settings);

    void addGuest(Client *client);
    void addClient(Client *client);
    void addOperator(Client *client);

    void delGuest(Client *client);
    void delClient(Client *client);
    void delOperator(Client *client);

    bool isMember(Client *client);
    bool isOperator(Client *client);
    bool isGuest(Client *client);

    void broadcastMessage(const std::string &message, Client *sender);

  private:
    std::string _channel_name;
    std::string _password;
    std::string _topic;
    std::string _topicSetter;

    ChannelSettings _channel_settings;

    ClientMap _clients;
    ClientMap _operators;
    ClientMap _guests;
};
