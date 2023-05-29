#include "Channel.hpp"

Channel::Channel(const std::string &name, const std::string &password, Client *superuser)
	: _name(name), _password(password), _superuser(superuser){
}

Channel::~Channel(){}
