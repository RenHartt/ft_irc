#include <Command.hpp>
#include <IrcError.hpp>
#include <Server.hpp>
#include <Utils.hpp>
#include <sched.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

void Command::_executeInvoke(Client *client, std::vector<std::string> &args)
{
    std::string message;

    if (args.size() < 2)
        throw IrcError(client->getNickname(), "invoke", CLIENT_NEEDMOREPARAMS);

    ClientMap client_list = _server->getClientsList();
    Client   *bot = client_list[_server->getClientFdByNickname(args[1])];
    if (bot)
    {
        message = ":" + client->getNickname() + "!" + client->getUsername() +
                  "@localhost PRIVMSG " + args[1] + " :!help\r\n";
        send(bot->getFd(), message.c_str(), message.size(), 0);
        return;
    }
	if (access("./ircbot", X_OK) == 0)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			const char *address = "127.0.0.1";
			std::string port    = itoa(_server->getPort());
			std::string password = _server->getPassword();
			const char *invoker = client->getNickname().c_str();
			const char *bot_name = args[1].c_str();
			const char *botExecutable = "./ircbot";
			const char *botArgs[] = {botExecutable, address, port.c_str(), password.c_str(), invoker, bot_name, NULL};
			
			char *env[2];
			pid_t serverPID = getpid();
			std::string pid = itoa(serverPID);
			env[0] = (char *)pid.c_str();
			env[1] = NULL;
			execvpe(botExecutable, (char *const *)botArgs, env);
		} else if (pid < 0)
			throw std::runtime_error("Erreur lors de la création du processus du bot.");
		else
		{
			message = args[1] + " successfully launched\r\n";
			send(client->getFd(), message.c_str(), message.size(), 0);
		}
	}
}
