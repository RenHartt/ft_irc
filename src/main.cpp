#include "Server.hpp"
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
   
	Server irc_server(argv[1]);
    irc_server.run();
    
	return (EXIT_SUCCESS);
}

