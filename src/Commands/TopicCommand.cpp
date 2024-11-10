#include <Channel.hpp>
#include <IrcError.hpp>
#include <Server.hpp>



void Command::_executeTopic(Client *client, std::vector<std::string> args)
{
	std::string sender_nickname = client->getNickname();

    if (args.size() < 2)
        throw IrcError(sender_nickname, "TOPIC", CLIENT_NEEDMOREPARAMS);

    std::string channel_name = args[1];
    Channel *channel = _server->getChannelsList()[channel_name];
    if (!channel)
        throw IrcError(sender_nickname, channel_name, CLIENT_NOSUCHCHANNEL);
    if (!channel->isMember(client))
        throw IrcError(sender_nickname, channel_name, CLIENT_NOTONCHANNEL);
    if (!channel->isOperator(client) && channel->channel_settings.t_topicRestriction)
        throw IrcError(sender_nickname, channel_name, CLIENT_CHANOPRIVSNEEDED);

    if (args.size() == 2)
    {
        std::string topic = channel->getTopic();
        if (topic.empty())
        {
            std::string response = ":" + _server->getName() + " 331 " + sender_nickname + " " + channel_name + " :No topic is set\r\n";
            send(client->getFd(), response.c_str(), response.size(), 0);
        }
        else
        {
            std::string response = ":" + _server->getName() + " 332 " + sender_nickname + " " + channel_name + " :" + topic + "\r\n";
            response += ":" + _server->getName() + " 333 " + sender_nickname + " " + channel_name + " " + channel->getTopicSetter() + "\r\n";
            send(client->getFd(), response.c_str(), response.size(), 0);
        }
        return;
    }

    std::string new_topic = args[2];

    channel->setTopic(new_topic);
    channel->setTopicSetter(sender_nickname);

    std::string confirmation = ":" + sender_nickname + "!" + client->getUsername() + "@" + "localhost" + " TOPIC " + channel_name + " :" + new_topic + "\r\n";
    channel->broadcastMessage(confirmation, client);
}


