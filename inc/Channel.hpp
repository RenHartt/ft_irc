#pragma once

#include <Client.hpp>
#include <Utils.hpp>
#include <stdint.h>

// need to be formatted with the letter that correspond first
// example : uint8_t f_foo() -> this will set the value of foo when f is send
struct ChannelSettings {
    uint8_t i_inviteOnly : 1;
    uint8_t t_topicRestriction : 1;
    uint8_t k_enableKey : 1;
    uint8_t l_userLimit;
};

class Client;

class Channel
{
  public:
    Channel(const std::string &channel_name);
    Channel(const std::string &channel_name, const std::string &password);

    std::string getChannelName(void) const;
    std::string getPassword(void) const;
    std::string getTopic(void) const;
    int         getNbClient(void) const;

    void setTopic(const std::string &new_topic);
    void setPassword(const std::string &password);

    void addClient(Client *client);
    void delClient(Client *client);

    void addOperator(Client *client);
    void delOperator(Client *client);

	void addGuest(Client *client);
	void delGuest(Client *client);

    bool isMember(Client *client);
    bool isOperator(Client *client);
	bool isGuest(Client *client);

    void broadcastMessage(const std::string &message, Client *sender);

    std::string getTopicSetter() const;
	void setTopicSetter(const std::string &topicSetter);
    ChannelSettings channel_settings;
    ClientMap       clients;
    ClientMap       operators;
	ClientMap		guests;

  private:
    std::string _channel_name;
    std::string _password;
    std::string _topic;
	std::string _topicSetter;
};
