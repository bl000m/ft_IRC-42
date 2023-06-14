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
		typedef std::map<std::string, Channel>	channelList;
		typedef std::map<std::string, Channel>::iterator channelListIt;
		typedef std::map<std::string, std::string>	channelModeList;
		typedef std::map<std::string, std::string>::iterator channelModeListIt;

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
		channelList						_channels;
		channelModeList					_channelModes;
		client_map						_clients;
		static fn_map const				_command;

		/*	command execution	*/
		fn_ptr		getCmd(std::string const &cmd);
		void		force_quit(int sock, bool err);

		std::vector<std::string>	splitCommands(std::string &buffer);

		/*	connection commands	*/
		void	pass(Client &client, Message const &mess);
		void	nick(Client &client, Message const &mess);
		void	user(Client &client, Message const &mess);
		void	quit(Client &client, Message const &mess);
		void	ping(Client &client, Message const &mess);
		void	pong(Client &client, Message const &mess);
		void	join(Client &client, Message const &mess);

		/* channel operators related commands */
		void 	invite(Client &client, Message const &mess);
		void 	kick(Client &client, Message const &mess);
		void 	topic(Client &client, Message const &mess);
		bool	setMode(Channel *channel, Client &client);
		bool 	parseChannelModes(const std::string& modeString, Message const &mess);
		void  handleKMode(channelModeListIt it, Channel* channel, Client& client);
		bool  handleOMode(channelModeListIt it, Channel* channel, Client& client);
		void  handleTMode(channelModeListIt it, Channel* channel, Client& client);
		void  handleIMode(channelModeListIt it, Channel* channel, Client& client);
		void  handleLMode(channelModeListIt it, Channel* channel, Client& client);
		std::string buildModeMessage(Channel* channel, const Client& client, const std::string& mode);

		/*	connection command helper	*/
		bool			nick_in_use(std::string const &nick) const;
		static bool		nick_valid(std::string const &nick);

		/*	privmsg and notice	*/
		void	privmsg(Client &client, Message const &mess);
		void	notice(Client &client, Message const &mess);
		bool	sendToNick(Client &client, Message const &mess, std::string const &nick);
		void	sendToChan(Client &client, Message const &mess, std::string const &chan);

		std::vector<std::string>	getTarget(std::string const &str);

		/*Channel related methods*/
		bool	createChan(std::string &name, std::string &pass, Client &client);
		void	joinChan(std::string &name, std::string &pass, Client &client);
		/* channel getter */
		Channel *getChannel(const std::string &channelName);

		/*	common reply	*/
		static void		reply(Client const &client, char const *cmd, char const *p1, char const *p2);
		void			broadcast(Client &source, char const *cmd, char const *p1, char const *p2);

		/*	static map initialization	*/
		static fn_map	cmd_init(void);

		/*	server queries	*/
		void	motd(Client &client, Message const &mess);
		void	wallops(Client &client, Message const &mess);
		void	kill(Client &client, Message const &mess);
		void	mode(Client &client, Message const &mess);
		void	oper(Client &client, Message const &mess);
		void	mode_user(Client &client, Message const &mess, std::string target);
		void	mode_channel(Client &client, Message const &mess, std::string target);
		void	who(Client &client, Message const &mess);
		void	who_chan(Client &client, std::string chan_name);
		void	who_nick(Client &client, std::string nick);
		void	who_mask(Client &client, std::string mask);
		std::string	who_reply(Client const &client, std::string nick, char const *chan);
		bool	share_chan(Client const &a, Client const &b);

		/*	client getter and remove	*/
		Client		*getClient(std::string const &nick);
		void		rmClient(Client &client);
		Channel		*getChan(std::string const &chan);
		void		rmChan(std::string const &chan);

};

void    closeSocket(pollfd &pfd);
void    sigExit(int code);

#endif
