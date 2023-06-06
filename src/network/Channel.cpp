#include "Channel.hpp"

Channel::Channel(Client *oper, std::string name)
	: _name(name) {
		user newChannelUser;
		std::string nickname = *(oper->getNick());
		newChannelUser.client = oper;
		newChannelUser.userMode = "o";
		newChannelUser.prefix = "@";
		_channelUsers[nickname] = newChannelUser;
}

Channel::~Channel(){}


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
	 _modes &= ~mode;  // Perform bitwise NOT and bitwise AND assignment
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




