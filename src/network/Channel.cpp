#include "Channel.hpp"

Channel::Channel(Client *oper, std::string name): _name(name) {
		if (!checkChannelName(name)) {
        throw std::invalid_argument("Invalid channel name: " + name);
    }
		user newChannelUser;
		std::string nickname = *(oper->getNick());
		newChannelUser.client = oper;
		newChannelUser.userMode = "o";
		newChannelUser.prefix = "@";
		_channelUsers[nickname] = newChannelUser;
}

Channel::~Channel(){}

void	Channel::addClient(Client *client){
		user newChannelUser;
		std::string nickname = *(client->getNick());
		newChannelUser.client = client;
		newChannelUser.userMode = "";
		newChannelUser.prefix = "+";
		_channelUsers[nickname] = newChannelUser;
}

// Check for spaces, control G / BELL, and comma in the channel name
bool Channel::checkChannelName(std::string channelName) {
    for (size_t counter = 0; i < channelName.length(); counter++) {
        if (channelName[counter] == ' ' || channelName[counter] == '\x07' || channelName[counter] == ',') {
            return false;
        }
    }
    return true;
}

/* --------------- Channel data related getters ---------------- */

std::string  Channel::getName(){
	return _name;
}

std::string Channel::getPassword(){
	return _password;
}

Channel::channelUsers Channel::getChannelUsers(){
	return _channelUsers;
}

size_t Channel::getUsersCount(){
	return _channelUsers.size();
}

/* --------------- Channel data related setters ---------------- */

void Channel::setPassword(std::string newPassword){
	_password = newPassword;
}




/*  <<<<<<<<<<<<<<<< SPECIFIC TO CHANNEL OPERATORS >>>>>>>>>>>>>>>>  */

/* --------------- mode ---------------- */

void	Channel::addMode(int mode){
	_modes |= mode;
}

void	Channel::removeMode(int mode){
	 _modes &= ~mode;
}

bool	Channel::hasMode(int mode){
	return (_modes & mode) == mode;
}

/* --------------- invite ---------------- */

void	Channel::inviteUser(std::string nickname){
	if (!invitedOrNot(nickname))
		_invitedUsers.push_back(nickname);
}

bool	Channel::invitedOrNot(std::string nickname){
	std::vector<std::string>::iterator it;
	for (it = _invitedUsers.begin(); it != _invitedUsers.end(); it++){
		if (*it == nickname)
			return true;
	}
	return false;
}

/* --------------- kick ---------------- */

void	Channel::removeUser(std::string nickname){
	std::vector<std::string>::iterator it;
	it = std::find(_invitedUsers.begin(), _invitedUsers.end(), nickname);
	if (it != _invitedUsers.end())
		_invitedUsers.erase(it);
}

/* --------------- topic ---------------- */

void	Channel::setTopic(std::string newTopic){
	_topic = newTopic;
}

std::string	Channel::getTopic(){
	return _topic;
}

/* --------------- broadcast ---------------- */
// to access privmsg we should make it static
// void 	Channel::broadcast(Message message){
// 	channelUsersIt it;
// 	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++){
// 		Server::privmsg(*(it->second.client), message);
// 	}
// }

