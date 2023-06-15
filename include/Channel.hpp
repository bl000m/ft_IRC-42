#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <iostream>
# include <vector>
# include <map>
# include "Client.hpp"
# include "Server.hpp"
# include "Message.hpp"


class Client;
class Message;
class Server;

typedef struct t_user{
	Client *client;
	std::string userMode;
	std::string prefix;
	time_t	joinedTime;
} user;

class Channel {


	public:
		typedef std::map<std::string, user> channelUsers;
		typedef std::map<std::string, user>::iterator channelUsersIt;

		Channel(Client *oper, std::string name);
		~Channel();
		bool	checkChannelName(std::string channelName);
		void	addClient(Client *client);
		bool	isUserInChannel(const std::string nickname);
		bool	isUserOperator(const std::string nickname);
		bool	isThereAnyOperator();
		std::string	getOldestMemberUser();
		void	setOldestMemberUserAsOperator();

		/* Channel data related getters*/
		const std::string&  	getName();
		std::string  	getPassword();
		channelUsers 	getChannelUsers();
		size_t			getUsersCount();
		int				getMemberLimit();
		std::string		getClientList();


		/* Channel data related setters*/
		void setPassword(std::string newPassword);
		void setUserAsOperator(std::string nickname);
		void removeUserAsOperator(std::string nickname);
		void setMemberLimit(const std::string& limit);

		/* commands specific to channel operators */

		/* mode */
		void	addMode(char mode);
		void	removeMode(char mode);
		bool	hasMode(char mode);
		std::string getMode();
		// bool	setMode(std::string mode);

		/* invite */
		void	inviteUser(std::string nickname);
		bool	isInvited(std::string nickname);
		void	removeInvitedUser(std::string nickname);

		/* kick */
		void	removeChannelUser(std::string nickname);

		/* topic */
		void	setTopic(std::string newTopic, std::string nickname);
		std::string	getTopic();
		std::string getTimeCreationTopic();
		std::string getNickCreationTopic();

		/* communicating */
		void broadcast(std::string message, Client client);
		void broadcastSenderIncluded(std::string message);

		/* utils */
		std::string getCurrentTime();
		time_t getCurrentTimeT();

	private:
		Channel();

		channelUsers				_channelUsers;
		std::vector<std::string>	_invitedUsers;
		std::string 				_name;
		std::string 				_password;
		std::string					_topic;
		std::string					_topicCreationTime;
		std::string					_topicCreatedBy;
		int							_modes;
		int							_memberLimit;
};
#endif
