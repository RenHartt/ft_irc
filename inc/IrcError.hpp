#include <Client.hpp>
#include <map>
#include <string>
#include <sys/types.h>

enum e_IrcErrorType {
    NONE,
    SERVER,
    SERVER_INIT,
    CLIENT,
    CLIENT_NONICKNAMEGIVEN,
    CLIENT_ALREADYREGISTERED,
    CLIENT_PASSWDMISMATCH,
    CLIENT_NOTREGISTERED,
    CLIENT_NORECIPIENT,
    CLIENT_NOTEXTTOSEND,
    CLIENT_NEEDMOREPARAMS,
    CLIENT_NOTONCHANNEL,
    CLIENT_CHANNELISFULL,
    CLIENT_BADCHANNELKEY,
    CLIENT_INVITEONLYCHAN,
    CLIENT_NOSUCHCHANNEL,
    CLIENT_CHANOPRIVSNEEDED,
    CLIENT_BADCHANMASK,
    CLIENT_CANNOTSENDTOCHAN,
    CLIENT_NICKNAMEINUSE,
    CLIENT_ERRONEUSNICKNAME,
    CLIENT_UNKNOWNCOMMAND,
    CLIENT_NOSUCHNICK,
    CLIENT_USERNOTINCHANNEL,
    CLIENT_USERONCHANNEL,
};

typedef std::map<enum e_IrcErrorType, std::string> MapError;

class IrcError
{

  public:
    IrcError(const std::string &msg, e_IrcErrorType type);
    IrcError(const std::string &msg, const std::string &msg2, e_IrcErrorType type);
    IrcError(const std::string &msg, const std::string &msg2, const std::string &msg3, e_IrcErrorType type);

    e_IrcErrorType getIrcErrorType(void) const;
    std::string    getMsg(void) const;

    void    log(void) const;
    ssize_t sendto(const Client &sender) const;

  private:
    std::string              _msg;
    e_IrcErrorType           _type;
    static const std::string _map_error[];
};
