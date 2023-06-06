#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <iostream>
# include <vector>
# include <map>
# include "Client.hpp"
# include "Server.hpp"


class Client;

	/* Memo Channel Operations - mqndatory :
	∗ KICK - Eject a client from the channel
	∗ INVITE - Invite a client to a channel
	∗ TOPIC - Change or view the channel topic
	∗ MODE - Change the channel’s mode:
	*/


typedef struct t_user{
	Client *client;
	std::string userMode;
	std::string prefix;
} user;

class Channel {


	public:
		#define t 1 << 1  // Bit 1 represents t mode => Set/remove the restrictions of the TOPIC command to channel operators
		#define k 1 << 2  // Bit 2 represents k mode => Set/remove the channel key (password)
		#define o 1 << 3  // Bit 3 represents o mode => Give/take channel operator privilege
		#define l 1 << 4  // Bit 4 represents l mode => Set/remove the user limit to channel
		#define i 1 << 5  // Bit 5 represents i mode => Set/remove Invite-only channel
		typedef std::map<std::string, user> channelUsers;
		typedef std::map<std::string, user>::iterator channelUsersIt;

		Channel(Client *oper, std::string name);
		~Channel();
		bool checkChannelName(std::string channelName);
		void	addClient(Client *client);
		// check name

		/* Channel data related getters*/
		std::string  getName();
		std::string  getPassword();
		channelUsers getChannelUsers();

		/* Channel data related setters*/
		void setPassword(std::string newPassword);

		/* commands specific to channel operators */

		/* mode */
		void	addMode(int mode);
		void	removeMode(int mode);
		bool	hasMode(int mode);

		/* invite */
		void	inviteUser(std::string nickname);
		bool	invitedOrNot(std::string nickname);
		void	removeUser(std::string nickname);

		/* kick */
		void	removeUser(Client *client);

		/* topic */
		void	setTopic(std::string newTopic);
		std::string	getTopic();

		/* communicating */
		void broadcast(std::string message);

	private:
		Channel();

		channelUsers				_channelUsers;
		std::vector<std::string>	_invitedUsers;
		std::string 				_name;
		std::string 				_password;
		std::string					_topic;
		int							_modes;
};

#endif
