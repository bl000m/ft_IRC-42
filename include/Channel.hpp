#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include "Client.hpp"

class Client;

class Channel {

	public:
		Channel(const std::string &name, const std::string &password, Client *superuser);
		~Channel();

	private:
		std::string _name;
		std::string _password;
		Client *_superuser;
		// std::vector<Client *> _clients
};

#endif
