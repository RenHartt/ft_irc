#include <Command.hpp>
#include <IrcError.hpp>

CommandMap Command::_commands = Command::_initCommands();

Command::Command(Server *server) { _server = server; }

CommandMap Command::_initCommands()
{
    CommandMap commands;
	commands["KICK"]    = &Command::_executeKick;
    commands["NICK"]    = &Command::_executeNick;
    commands["USER"]    = &Command::_executeUser;
    commands["PRIVMSG"] = &Command::_executePrivmsg;
    commands["JOIN"]    = &Command::_executeJoin;
    commands["PART"]    = &Command::_executePart;
    commands["HELP"]    = &Command::_executeHelp;
    commands["TOPIC"]   = &Command::_executeTopic;
	commands["INVITE"]  = &Command::_executeInvite;
	commands["PASS"]    = &Command::_executePass;
	commands["MODE"]    = &Command::_executeMode;
    commands["DIE"]     = &Command::_executeDie;

    return commands;
}

void Command::exec(const std::string &cmd, Client *client, std::vector<std::string> args)
{
    CmdAddr func = _find(cmd);
    if (!func)
        return;

    try
    {
        (this->*func)(client, args);
    } catch (const IrcError &e)
    {
        e.log();
        e.sendto(*client);
    }
}

CmdAddr Command::_find(const std::string &cmd) { return _commands.find(cmd) != _commands.end() ? _commands.find(cmd)->second : NULL; }
