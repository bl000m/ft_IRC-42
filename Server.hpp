//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER

#ifndef SERVER_HPP
# define SERVER_HPP

#include <stdexcept>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

class Server {

    public:
        Server(const std::string &port, const std::string &password);
        ~Server();

    private:
        bool                    newPoll(void);

        std::string             _port;
        std::string             _password;
        unsigned short          _iport;
        std::vector<pollfd>     _server_sockets;
};

void    closeSocket(pollfd pfd);

#endif