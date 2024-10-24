#include <Command.hpp>
#include <iostream>

/* init static  */

CommandMap Command::_commands = Command::_initCommands();

Command::Command(Server *server) { _server = server; }
/* constructor */


/* static function */

CommandMap Command::_initCommands()
{
    CommandMap commands;
    commands["NICK"] = &Command::_executeNick;
    commands["USER"] = &Command::_executeUser;
    commands["PRIVMSG"] = &Command::_executePrivmsg;
    commands["JOIN"] = &Command::_executeJoin;
    commands["PART"] = &Command::_executePart;
    commands["QUIT"] = &Command::_executeQuit;
	commands["HELP"] = &Command::_executeHelp;
	commands["WHOAMI"] = &Command::_executeWhoami;
    return commands;
}

/* function */

void Command::exec(const std::string &cmd, Client *client,
                   std::vector<std::string> args)
{
    CmdAddr func = _find(cmd);

    if (func)
        (this->*func)(client, args);
    else
        std::cerr << "Wrong function" << std::endl;
}

/* internal function */

CmdAddr Command::_find(const std::string &cmd)
{
    CommandMap::iterator it;

    it = _commands.find(cmd);
    if (it == _commands.end())
        return (NULL);
    else
        return (it->second);
}
