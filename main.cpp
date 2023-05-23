//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER 

#include Server.hpp

int main(int argc, char **argv){
    try {

		if (argc != 3) {
			throw std::invalid_argument("Please type: ./ircserv <port> <password>");
		}
        /* here or in Server constructor: */
        // check argv[1] numeric and < max limit ports
        // check argv[2] valid password
		Server server(argv[1], argv[2]);
		return 0;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}