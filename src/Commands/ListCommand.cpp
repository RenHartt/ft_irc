#include "Command.hpp"
#include "ErrTable.hpp"
#include "IrcError.hpp"
#include "Server.hpp"

#include <sstream>
#include <sys/types.h>

void Command::_executeList(Client *client, std::vector<std::string>)
{
    std::ostringstream   oss;
    std::string          list;
    ChannelMap           channel_list = _server->getChannelsList();
    ChannelMap::iterator it = channel_list.begin();

    std::string client_nickname = client->getNickname();

    // RPL_LISTSTART
    list += "321 " + client_nickname + " Channel :Users  Name\r\n";

    while (it != channel_list.end())
    {
        Channel    *channel = it->second;
        int         member_count = channel->getNbClient();
        std::string topic = channel->getTopic();  

        oss.str("");
        oss << member_count;
        list += "322 " + client_nickname + " " + channel->getChannelName() + " " +
                oss.str() + " :" + topic + "\r\n";
        ++it;
    }

    list += "323 " + client_nickname + " :End of /LIST\r\n";

ssize_t bytes_sent = send(client->getFd(), list.c_str(), list.size(), 0);
if (bytes_sent != (ssize_t)list.size()) {
    std::cerr << "Erreur : envoi incomplet de la liste des canaux" << std::endl;
}
}
