#pragma once

#define ERR_NONICKNAMEGIVEN(source)                    (std::string("431 ") + source + " :Nickname not given")
#define ERR_ALREADYREGISTERED(source)                  (std::string("462 ") + source + " :You may not reregister")
#define ERR_PASSWDMISMATCH(source)                     (std::string("464 ") + source + " :Password incorrect")
#define ERR_NOTREGISTERED(source)                      (std::string("451 ") + source + " :You have not registered")
#define ERR_NORECIPIENT(nickname)						("411 " + nickname + " :No recipient given (PRIVMSG)\r\n")
#define ERR_NOTEXTTOSEND(nickname)						("412 " + nickname + " :No text to send\r\n")

#define ERR_NEEDMOREPARAMS(source, command)				(std::string("461 ") + source + " " + command + " :Not enough parameters")
#define ERR_NOTONCHANNEL(source, channel)				(std::string("442 ") + source + " " + channel + " :You're not on that channel")
#define ERR_CHANNELISFULL(source, channel)			   (std::string("471 ") + source + " " + channel + " :Cannot join channel (+l)")
#define ERR_BADCHANNELKEY(source, channel)             (std::string("475 ") + source + " " + channel + " :Cannot join channel (+k)")
#define ERR_INVITEONLYCHAN(source, channel)            (std::string("473 ") + source + " " + channel + " :Cannot join channel (+i)")
#define ERR_NOSUCHCHANNEL(source, channel)             (std::string("403 ") + source + " " + channel + " :No such channel")
#define ERR_CHANOPRIVSNEEDED(source, channel)          (std::string("482 ") + source + " " + channel + " :You're not channel operator")
#define ERR_BADCHANMASK(source, channel)               (std::string("476 ") + source + " " + channel + " :Bad Channel Mask")
#define ERR_CANNOTSENDTOCHAN(source, channel)          (std::string("404 ") + source + " " + channel + " :Cannot send to channel")
#define ERR_NICKNAMEINUSE(source, nickname)            (std::string("433 ") + source + " " + nickname + " :Nickname is already in use")
#define ERR_ERRONEUSNICKNAME(source, nickname)			(std::string("432 ") + source + " " + nickname + " :Erroneous nickname")
#define ERR_UNKNOWNCOMMAND(source, command)            (std::string("421 ") + source + " " + command + " :Unknown command")
#define ERR_NOSUCHNICK(source, name)                   (std::string("401 ") + source + " " + name + " :No such nick/channel")

#define ERR_USERNOTINCHANNEL(source, user, channel)		(std::string("441 ") + source + " " + user + " " + channel + " :They aren't on that channel")
#define ERR_USERONCHANNEL(source, target, channel)     (std::string("443 ") + source + " " + target + " " + channel + " :is already on channel")
#define RPL_LIST(source, client, channel, num_users, topic) \
    (std::string("322 ") + source + " " + client + " " + channel + " " + std::to_string(num_users) + " :" + topic + "\r\n")

#define RPL_LISTEND(source, client) (std::string("323 ") + source + " " + client + " :End of /LIST\r\n")

