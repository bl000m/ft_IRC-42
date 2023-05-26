//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER

#ifndef SERVER_HPP
# define SERVER_HPP

#include <stdexcept>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "global.hpp"

#include "Message.hpp"
#include "Client.hpp"
#include "CommandNum.hpp"

#define MAX_QUEUE_CONNECTION    42
#define POLL_DELAY              5
#define MIN_PORT                1023
#define MAX_PORT                65535
#define MIN_ARGC                3
#define MAX_BUFFER              512
#define CLOSE_SOCKET            0
#define CMD_NUM					3 //should increase in future

#define IS_POLLIN(revents)      (revents & POLLIN)
#define IS_POLLHUP(revents)     (revents & POLLHUP)

class Server {
    public:
        Server();
        ~Server();

        void    initServer(const std::string &port, const std::string &password);
        void    run(void);
		void	execMessage(Client const &client, Message const &mess);

    private:
        bool                    initServerPoll(void);
        bool                    newClientPoll(void);

        std::string             _password;
        uint16_t                _iport;

        sockaddr_in             _addr;
        std::vector<pollfd>     _server_sockets;
		std::map<int, Client>	_clients;

		/*	commands	*/
		void	(*cmd[CMD_NUM])(Client const &client, Message const &mess);
		void	pass(Client const &client, Message const &mess);
		void	nick(Client const &client, Message const &mess);
		void	user(Client const &client, Message const &mess);
};

void    closeSocket(pollfd pfd);
void    sigExit(int code);

#endif