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

		/* getters */
		std::string &getName();
		std::string &getPassword();
		Client *getSuperser();

		/* setters */
		void setPassword(std::string newPassword);

		/* commands support function */
		void	addUser(Client *user);
		void	removeUser(Client *user);
		std::vector<std::string> getNames();

	private:
		Channel();
		std::string _name;
		std::string _password;
		Client *_superuser;
		Channel	&operator=(Channel const &channel);
		void clear();
		std::vector<Client *> _clients;
};

#endif
