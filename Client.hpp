#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <iostream>
# include <vector>

/*
	The default ctor creates an empty client, with -1 as sock

	The Client(int sockfd) creates a client with socket, which
	should be added to a container by the server

	If a client quit, the server should remove it from the 
	container, so the resources can be released
*/
class Client
{
	public:
		/*	canon form and ctor	*/
		Client(void);
		Client(Client const &client);
		~Client(void);
		Client	&operator=(Client const &client);

		Client(int sockfd, unsigned int ip);
		
		/*	setters	*/
		void	bePassed(void);
		void	setNick(std::string const &nick);
		void	setUser(std::string const &user);
		void	setHost(std::string const &host);

		/*	getters	*/
		int					getSock(void) const;
		unsigned int		getIp(void) const;
		bool				isPassed(void) const;
		std::string const	*getNick(void) const;
		std::string const	*getUser(void) const;
		std::string const	*getHost(void) const;
		bool				isRegist(void) const;

	private:
		/*	private variable	*/
		int				_sock;
		unsigned int	_ip;
		bool			_pass;
		std::string		*_nick;
		std::string		*_user;
		std::string		*_host;

		/*	private function	*/
		void	clear(void);
};

std::ostream	&operator<<(std::ostream &out, Client const &client);


#endif