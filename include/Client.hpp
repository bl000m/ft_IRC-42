#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <cstring>
# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include "Channel.hpp"

class Channel;
/*
	The default ctor creates an empty client

	The ctor Client(int sockfd, unsigned int ip) creates a connected client, which should be added to a container by the server

	If a client quit, the server should remove it from the
	container, so the resources can be released
*/

/*
	The varaiable _pass and _regist:

	The client must first send PASS command, other commands will be ignored or regarded as unknown.The client can sent multiple PASS, only the value of last one will be used.

	The client can then send NICK, NIck can be set multiple time.

	The client can then set USER, once set cannot be modified.

	Once enter NICK command, the PASS will no longer be accepted.
	Once client fully registered, the_regist will be true. PASS USER will no longer be accepted
*/
class Client
{
	public:
		typedef std::map<std::string, Channel*> channelMap;
		typedef std::map<std::string, Channel*>::iterator channelMapIt;
		/*	canon form and ctor	*/
		Client(void);
		Client(Client const &client);
		~Client(void);
		Client	&operator=(Client const &client);

		Client(int sockfd, socklen_t sock_size, sockaddr_in sock_addr);

		/*	reply-to-client related function	*/
		void	reply(char const *numeric, char const *p1, char const *p2);
		void	reply(char *src, char *cmd, char *para1, char *para2);
		void	reply(char *src, char *cmd, char *para1, char *para2, char *para3);
		void	reply(char const *note);
		void	beSent(void);

		/*	setters	*/
		void	setPass(bool good);
		void	beRegist(void);
		void	setNick(std::string const &nick);
		void	setUser(std::string const &user);
		void	setHost(std::string const &host);
		void	setServerOp(bool yes);
		void	setInvisible(bool yes);
		void	setWallop(bool yes);
		bool	setMode(std::string mode);
		void	beQuit(void);
		void	setAway(std::string mess);


		/*	getters	*/
		int		getSock(void) const;
		bool	getPass(void) const;
		bool	isRegist(void) const;
		bool	isServerOp(void) const;
		bool	isInvisible(void) const;
		bool	getWallop(void) const;
		bool	isQuit(void) const;
		std::string	getAway(void) const;
		channelMap getChannels() const;

		/* channel related */
		void	addChannel(Channel *newChannel, std::string channelName);
		void	removeChannel(const std::string &channelName);

		std::string const	*getNick(void) const;
		std::string const	*getUser(void) const;
		std::string const	*getHost(void) const;
		std::string			getFullName(void) const;
		std::string			getMode(void) const;
		std::string			&getBuff(void);
		void				clearBuff(void);
		void				catBuff(char *buff, int size);

	private:
		/*	user mode	*/
		bool			_regist;
		bool			_invisible;
		bool			_server_op;
		bool			_wallop;

		/*	private variable	*/
		int				_sock;
		bool			_pass;
		std::string		*_nick;
		std::string		*_user;
		std::string		*_host;
		socklen_t		_sock_len;
		sockaddr_in		_sock_addr;
		channelMap		_channelsMember;
		std::string		_readbuf;
		std::string		_envelope;
		bool			_quit;
		std::string		_away;
		/*	private function	*/
		void	clear(void);
};

std::ostream	&operator<<(std::ostream &out, Client const &client);


#endif
