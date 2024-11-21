#include <Command.hpp>
#include <Server.hpp>

void Command::_executeQuit(Client *, std::vector<std::string> &)
{
	/* _server->delClient(sender->getFd()); */
	_server->broadcastServer("QUIT :Leaving");
}
