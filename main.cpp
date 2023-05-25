//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER 

#include "Server.hpp"

int main(int argc, char **argv)
{
	Server server;

    /* here or in Server constructor: */
    // check argv[1] numeric and < max limit ports
    // check argv[2] valid password
	if (argc != MIN_ARGC)
	{
		std::cerr << "Please type: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	try
	{
		server.initServer(argv[1], argv[2]);
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	server.run();
	return (0);
}