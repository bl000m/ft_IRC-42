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

/* --------------- invite ---------------- */

void	Channel::inviteUser(std::string nickname){
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

/* --------------- topic ---------------- */


void	Channel::removeUser(Client *client){
	std::vector<Client *>::iterator it;
	it = std::find(_clients.begin(), _clients.end(), user);
	if (it != _clients.end())
		_clients.erase(it);
	// still to check if _clients empty and if user = _superuser
}


