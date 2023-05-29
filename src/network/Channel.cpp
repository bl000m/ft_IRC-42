#include "Channel.hpp"

Channel::Channel(const std::string &name, const std::string &password, Client *superuser)
	: _name(name), _password(password), _superuser(superuser){
}

Channel::~Channel(){}

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
