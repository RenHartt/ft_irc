#include <Channel.hpp>
#include <IrcError.hpp>
#include <Server.hpp>



void Command::_executeTopic(Client *client, std::vector<std::string> args)
{
    if (args.size() < 2)
        throw IrcError(client->getNickname(), "TOPIC", CLIENT_NEEDMOREPARAMS);

    std::string channel_name = args[1];
    ChannelMap channel_list = _server->getChannelsList();
    ChannelMap::iterator it = channel_list.find(channel_name);

    if (it == channel_list.end())
        throw IrcError(client->getNickname(), channel_name, CLIENT_NOSUCHCHANNEL);

    Channel *channel = it->second;

    if (!channel->isMember(client))
        throw IrcError(client->getNickname(), channel_name, CLIENT_NOTONCHANNEL);

    if (args.size() == 2)
    {
        std::string topic = channel->getTopic();
        if (topic.empty())
        {
            std::string response = ":" + _server->getName() + " 331 " + client->getNickname() + " " + channel_name + " :No topic is set\r\n";
            send(client->getFd(), response.c_str(), response.size(), 0);
        }
        else
        {
            std::string response = ":" + _server->getName() + " 332 " + client->getNickname() + " " + channel_name + " :" + topic + "\r\n";
            send(client->getFd(), response.c_str(), response.size(), 0);
            std::string topic_info = ":" + _server->getName() + " 333 " + client->getNickname() + " " + channel_name + " " + channel->getTopicSetter() + "\r\n";
            send(client->getFd(), topic_info.c_str(), topic_info.size(), 0);
        }
        return;
    }

    if (!channel->isOperator(client) && channel->channel_settings.t_topicRestriction)
        throw IrcError(client->getNickname(), channel_name, CLIENT_CHANOPRIVSNEEDED);

    std::string new_topic = args[2];
    channel->setTopic(new_topic);
    channel->setTopicSetter(client->getNickname());

    std::string confirmation = ":" + client->getNickname() + "!" + client->getUsername() + "@" + "localhost" + " TOPIC " + channel_name + " :" + new_topic + "\r\n";
    channel->broadcastMessage(confirmation, client);
}


