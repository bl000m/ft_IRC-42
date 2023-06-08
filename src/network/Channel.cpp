#include "Channel.hpp"

 /*
 	Initializes a new channel with a name and an operator (oper).
	Checks if the channel name is valid.
	Adds the operator to the channel's user list with operator privileges.
*/
Channel::Channel(Client *oper, std::string name): _name(name), _modes(0) {
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

 /*
	Creates a new channel user with default user mode and prefix.
	Inserts the client into the channel's user list.
 */
void	Channel::addClient(Client *client){
		user newChannelUser;
		std::string nickname = *(client->getNick());
		newChannelUser.client = client;
		// newChannelUser.userMode = "";
		newChannelUser.prefix = "+";
		_channelUsers[nickname] = newChannelUser;
}

 /*
	Checks if the channel name is valid.
	Returns true if the channel name does not contain spaces, control G/BELL characters, or commas.
	Returns false otherwise.
 */
bool Channel::checkChannelName(std::string channelName) {
    for (size_t counter = 0; counter < channelName.length(); counter++) {
        if (channelName[counter] == ' ' || channelName[counter] == '\x07' || channelName[counter] == ',') {
            return false;
        }
    }
    return true;
}

/* --------------- Channel data related getters ---------------- */

/*
	Returns the name of the channel.
*/
const std::string& Channel::getName(){
	return _name;
}

/*
	Returns the password of the channel.
*/
std::string Channel::getPassword(){
	return _password;
}

/*
	Returns the list of users in the channel.
*/
Channel::channelUsers Channel::getChannelUsers(){
	return _channelUsers;
}

/*
	Returns the number of users in the channel.
*/
size_t Channel::getUsersCount(){
	return _channelUsers.size();
}

/* --------------- Channel data related setters ---------------- */

/*
	Sets a new password for the channel.
*/
void Channel::setPassword(std::string newPassword){
	_password = newPassword;
}

void Channel::setUserAsOperator(std::string nickname){
	channelUsersIt it;
	it = _channelUsers.find(nickname);
    if (it != _channelUsers.end())
        it->second.prefix = "@";
}


/*  <<<<<<<<<<<<<<<< SPECIFIC TO CHANNEL OPERATORS >>>>>>>>>>>>>>>>  */

/* --------------- mode ---------------- */

/*
	Adds a mode to the channel.
    Updates the channel's mode by integrating the existing mode with the new mode (OR operation).
*/
void Channel::addMode(char mode) {
    switch (mode) {
        case 't':
            _modes |= (1 << 0);
            break;
        case 'k':
            _modes |= (1 << 1);
            break;
        case 'o':
            _modes |= (1 << 2);
            break;
        case 'l':
            _modes |= (1 << 3);
            break;
        case 'i':
            _modes |= (1 << 4);
            break;
        default:
            break;
    }
}

/*
	Removes a mode from the channel.
    Updates the channel's mode by updating the existing mode with the complement of the new mode (AND operation).
*/
void Channel::removeMode(char mode) {
    switch (mode) {
        case 't':
            _modes &= ~(1 << 0);
            break;
        case 'k':
            _modes &= ~(1 << 1);
            break;
        case 'o':
            _modes &= ~(1 << 2);
            break;
        case 'l':
            _modes &= ~(1 << 3);
            break;
        case 'i':
            _modes &= ~(1 << 4);
            break;
        default:
            break;
    }
}

/*
	Checks if the channel has a specific mode.
    Returns true if the channel's mode has all the bits of the specified mode set.
    Returns false otherwise.
*/
bool Channel::hasMode(char mode) {
    switch (mode) {
        case 't':
            return (_modes & (1 << 0)) != 0;
        case 'k':
            return (_modes & (1 << 1)) != 0;
        case 'o':
            return (_modes & (1 << 2)) != 0;
        case 'l':
            return (_modes & (1 << 3)) != 0;
        case 'i':
            return (_modes & (1 << 4)) != 0;
        default:
            return false;
    }
}


/* --------------- invite ---------------- */

/*
	Invites a user to the channel.
    Adds the user's nickname to the list of invited users.
*/
void	Channel::inviteUser(std::string nickname){
	if (!isInvited(nickname))
		_invitedUsers.push_back(nickname);
}

/*
	Checks if a user is invited to the channel.
    Returns true if the user's nickname is found in the list of invited users.
    Returns false otherwise.
*/
bool	Channel::isInvited(std::string nickname){
	std::vector<std::string>::iterator it;
	for (it = _invitedUsers.begin(); it != _invitedUsers.end(); it++){
		if (*it == nickname)
			return true;
	}
	return false;
}

/*
    Removes a user from the list of invited users.
*/
void	Channel::removeInvitedUser(std::string nickname){
	std::vector<std::string>::iterator it;
	it = std::find(_invitedUsers.begin(), _invitedUsers.end(), nickname);
	if (it != _invitedUsers.end())
		_invitedUsers.erase(it);
}

/* --------------- kick ---------------- */

/*
    Removes a user from the list of channel users.
*/
void	Channel::removeChannelUser(std::string nickname){
	channelUsersIt it;
	it = _channelUsers.find(nickname);
    if (it != _channelUsers.end())
        _channelUsers.erase(it);
}

/* --------------- topic ---------------- */

/*
	Sets a new topic for the channel.

*/
void	Channel::setTopic(std::string newTopic){
	_topic = newTopic;
}

/*
	Returns the topic of the channel.

*/
std::string	Channel::getTopic(){
	return _topic;
}

/* --------------- broadcast ---------------- */
// to access privmsg it should be public
void 	Channel::broadcast(Message message, Client client, Server server){
	channelUsersIt it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++){
		if (it->first != *(client.getNick()))
			server.privmsg(*(it->second.client), message);
	}
}

