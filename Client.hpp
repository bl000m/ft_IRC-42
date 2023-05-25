#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <iostream>
# include <vector>

class Client
{
	public:
		/*	member type	*/
		typedef struct s_client					t_client;
		typedef std::map<std::string, t_client>	client_map;

		/*	canon form	*/
		Client(void);
		Client(Client const &client);
		~Client(void);
		Client	&operator=(Client const &client);

		/*	member function	*/
		bool				regist(int sock);
		std::string const	&getNick(void) const;
		std::string const	&getReal(void) const;
		std::string const	&getUser(void) const;
		client_map const	&getWhoswho(void) const;

	private:
		/*	private variable	*/
		t_client			_info;

		/*	private function	*/
		void	clear(void);

		/*	static data	*/
		static client_map	_whos_who;

};

std::ostream	&operator<<(std::ostream &out, Client const &client);


#endif