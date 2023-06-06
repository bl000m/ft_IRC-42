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
#include <map>
#include "global.hpp"

#include "Message.hpp"
#include "Client.hpp"
#include "Numerics.hpp"
#include "Channel.hpp"

#define MAX_QUEUE_CONNECTION    42
#define POLL_DELAY              5
#define MIN_PORT                1023
#define MAX_PORT                65535
#define MIN_ARGC                3
#define MAX_BUFFER              512
#define CLOSE_SOCKET            0

#define IS_POLLIN(revents)      (revents & POLLIN)
#define IS_POLLHUP(revents)     (revents & POLLHUP)

class Channel;

class Server {
    public:
		/*	typedef	*/
		typedef void (Server::*fn_ptr) (Client &c, Message const &m);
		typedef std::map<int, Client>			client_map;
		typedef std::map<std::string, fn_ptr>	fn_map;

        Server();
        ~Server();

        void    initServer(const std::string &port, const std::string &password);
        void    run(void);
		void	execMessage(Client &client, Message const &mess);

    private:
        bool					initServerPoll(void);
        bool					newClientPoll(void);

        std::string						_password;
        uint16_t						_iport;

        sockaddr_in						_addr;
        std::vector<pollfd>				_server_sockets;
		std::map<std::string, Channel>	_channels;
		client_map						_clients;
		static fn_map const				_command;

		/*	command execution	*/
		fn_ptr		getCmd(std::string const &cmd);
		void		force_quit(int sock);

		
		/*	connection commands	*/
		void	pass(Client &client, Message const &mess);
		void	nick(Client &client, Message const &mess);
		void	user(Client &client, Message const &mess);
		void	quit(Client &client, Message const &mess);
		void	ping(Client &client, Message const &mess);
		void	pong(Client &client, Message const &mess);
		void	join(Client &client, Message const &mess);

		/*	connection command helper	*/
		static void		welcome_mess(Client const &client);
		bool			nick_in_use(std::string const &nick) const;
		static bool		nick_valid(std::string const &nick);

		/*	privmsg and notice	*/
		void	privmsg(Client &client, Message const &mess);
		void	notice(Client &client, Message const &mess);
		bool	sendToNick(Client &client, Message const &mess, std::string const &nick);
		std::vector<std::string>	getTarget(std::string const &str);

		/*Channel related methods*/
		void	createChan(std::string &name, std::string &pass, Client &client);
		void	joinChan(std::string &name, std::string &pass, Client *client);
		void	checkChan(std::string &name);

		/*	common reply	*/
		static void		reply(Client const &client, char const *cmd, char const *p1, char const *p2);
		void			broadcast(Client const &source, char const *cmd, char const *p1, char const *p2);

		/*	static map initialization	*/
		static fn_map	cmd_init(void);

		/*	should not be commited	*/
		void	motd(Client &client, Message const &mess);
		void	wallops(Client &client, Message const &mess);
		void	kill(Client &client, Message const &mess);
		void	mode(Client &client, Message const &mess);
		void	oper(Client &client, Message const &mess);
		void	mode_user(Client &client, Message const &mess, std::string target);

		/*	client getter and remove	*/
		Client	*getClient(std::string const &nick);
		void	rmClient(Client &client);
};

void    closeSocket(pollfd &pfd);
void    sigExit(int code);

#endif