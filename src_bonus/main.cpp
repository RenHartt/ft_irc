#include <cstdlib>
#include <iostream>
#include "Bot.hpp"
#include <signal.h>

bool running = true;

void handleSignal(int signal)
{
	if (signal == SIGINT)
		running = false;
}

int main(int argc, char **argv )
{
	if (argc != 6)
	{
		std::cerr << "Usage : ./Bot <server address> <port> <password> <bot name>" << std::endl;
		return 1;
	}

	signal(SIGINT, handleSignal);

	Bot bot(argv[1], std::atoi(argv[2]), argv[3], argv[4], argv[5]);

	try
	{
		bot.connectToServer();
		bot.authenticate();
		bot.listenForMessages();
	} catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
