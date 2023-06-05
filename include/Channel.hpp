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
		Channel();
		~Channel();

		/* kick  */
		void removeUser(Client *user);

		/* invite */
		void addUser(Client *user);

		/* topic */
		void setTopic(std::string newTopic);
		std::string getTopic();

		/* mode */
		void setMode(std::string mode);
		std::string getMode();

		std::string &getName();
		std::string &getPassword();

		Client *getSuperser();

		void setPassword(std::string newPassword);
		std::string getPassword();

		/* commands support function */
		std::vector<std::string> getNames();
		void broadcast(Client &user, std::string message);

	private:
		Channel();
		std::string				_name;
		std::string 			_password;
		std::string 			_topic;
		std::string 			_mode;
		std::vector<Client *> 	_clients;
		std::vector<Client *> 	_operators;
		std::vector<Client *> 	_banned;
		std::vector<Client *> 	_invited;
		Client *_superuser;
		Channel	&operator=(Channel const &channel);
		void clear();
};

#endif
