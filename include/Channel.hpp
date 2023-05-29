#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <iostream>
# include <vector>
#include "Client.hpp"
#include "Server.hpp"

class Client;

class Channel {

	public:
		Channel(const std::string &name, const std::string &password, Client *superuser);
		~Channel();

	private:
		Channel();
		std::string _name;
		std::string _password;
		Client *_superuser;
		Channel	&operator=(Channel const &channel);
		void clear();
		// std::vector<Client *> _clients
};

#endif
