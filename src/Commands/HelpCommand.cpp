#include <Command.hpp>
#include <Server.hpp>

void Command::_executeHelp(Client *client, std::vector<std::string>)
{
    std::string help = "\n\nAvailable commands:\n";
    help += "NICK <nickname>				: Set your nickname\n";
    help += "JOIN <#channel ...> <password>		: Join a channel\n";
    help += "LEAVE <#channel>			: Leave a channel\n";
    help += "LIST					: List all channels\n";
    help += "USERS <#channel>			: List all users in a channel\n";
    help += "MSG <#channel> <message>		: Send a message to a channel\n";
    help += "PRIVMSG <nickname> <message>		: Send a private message to a user\n";
    help += "HELP					: Display this help message\n";
    help += "QUIT					: Disconnect from the server\n\n";
    send(client->getFd(), help.c_str(), help.size(), 0);
}
