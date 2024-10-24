#include <Server.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
        return (EXIT_FAILURE);
    }

    Server irc_server(argv[1], argv[2]);
    irc_server.run();

    return (EXIT_SUCCESS);
}
