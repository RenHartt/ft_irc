#include <csignal>
#include <iostream>
#include <Server.hpp>
#include <IrcError.hpp>

sig_atomic_t server_running = 1;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
        return (EXIT_FAILURE);
    }

	signal(SIGINT, handleSignal);

	Server server(argv[1], argv[2]);
    try
    {
		server.init();
        server.run();
    } catch (const IrcError &e)
    {
        e.log();
        return (e.getIrcErrorType());
    }
    return (EXIT_SUCCESS);
}
