#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
   
	Server irc_server(atoi(argv[1]));
	irc_server.init();
	irc_server.run();
    
	return (EXIT_SUCCESS);
}

