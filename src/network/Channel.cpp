#include "Channel.hpp"

Channel::Channel(){
}

Channel::~Channel(){}


/* --------------- getters ---------------- */

std::string &Channel::getName(){
	return _name;
}

std::string &Channel::getPassword(){
	return _password;
}

Client *Channel::getSuperser(){
	return _superuser;
}


void Channel::setPassword(std::string newPassword){
	_password = newPassword;
}

/* --------------- commands support function---------------- */
void	Channel::addUser(Client *user){
	_clients.push_back(user);
}

void	Channel::removeUser(Client *user){
	std::vector<Client *>::iterator it;
	it = std::find(_clients.begin(), _clients.end(), user);
	if (it != _clients.end())
		_clients.erase(it);
	// still to check if _clients empty and if user = _superuser
}

std::vector<std::string> Channel::getNames(){
	std::vector<Client *>::iterator it;
	std::vector<std::string> nameList;
	for (it = _clients.begin(); it != _clients.end(); it++){
		Client *clientOnSpot = *it;
		std::string prefix = (_superuser == clientOnSpot ? "@" : "");
		std::string name = prefix + *(*it)->getNick();
		nameList.push_back(name);
	}
	return nameList;
}
/* --------------- modes ---------------- */
	std::string &getName(){

	}
/*
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege
· l: Set/remove the user limit to channel
*/

/* --------------- operator= overload (private) ---------------- */

Channel	&Channel::operator=(Channel const &channel){
	if (this == &channel)
		return (*this);
	_name = channel._name;
	_password = channel._password;
	_superuser = channel._superuser;
	clear();
	return (*this);
}

void	Channel::clear(){
	delete _superuser;
}
