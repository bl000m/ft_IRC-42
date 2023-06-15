#include "Channel.hpp"

 /*
 	Initializes a new channel with a name and an operator (oper).
	Checks if the channel name is valid.
	Adds the operator to the channel's user list with operator privileges.
*/
Channel::Channel(Client *oper, std::string name): _name(name), _topic(""), _modes(0), _memberLimit(1){
		user newChannelUser;
		std::string nickname = *(oper->getNick());
		newChannelUser.client = oper;
		newChannelUser.userMode = oper->getMode();
		newChannelUser.prefix = "@";
		newChannelUser.joinedTime = getCurrentTimeT();
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
		newChannelUser.userMode = client->getMode();
		newChannelUser.prefix = "+";
		newChannelUser.joinedTime = getCurrentTimeT();
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
	if (channelName[0] != '#')
		return (false);
    return true;
}

bool	Channel::isUserInChannel(const std::string nickname){
	channelUsersIt it;
	it = _channelUsers.find(nickname);
    if (it != _channelUsers.end())
        return true;
	else
		return false;
}

bool	Channel::isUserOperator(const std::string nickname){
	channelUsersIt it;
	it = _channelUsers.find(nickname);
	if (it->second.prefix == "@")
        return true;
	else
		return false;
}

bool	Channel::isThereAnyOperator(){
	channelUsersIt it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++){
		if (it->second.prefix == "@")
			return true;
	}
	return false;
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

int	Channel::getMemberLimit(){
	return _memberLimit;
}

std::string	Channel::getClientList()
{
	std::string	ret;

	for (channelUsers::iterator i = _channelUsers.begin(); i != _channelUsers.end(); i++)
	{
		if (isUserOperator(i->first))
			ret.append("@");
		ret.append(i->first + " ");
	}
	return ret;
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

std::string Channel::getOldestMemberUser(){
	channelUsersIt it;
	it = _channelUsers.begin();
	std::string oldestMemberUserNick;
	time_t isBefore = (it->second.joinedTime);
	oldestMemberUserNick = it->first;
	std::cout << "isBefore: " << isBefore << std::endl;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++){
		std::cout << "nick: "<< it->first << " joined time: " << it->second.joinedTime << std::endl;
		if (it->second.joinedTime < isBefore){
			isBefore = it->second.joinedTime;
			std::cout << "in IF isBefore: " << isBefore << std::endl;
			std::cout << "it->first = " << it->first << std::endl;
			oldestMemberUserNick = it->first;
		}
	}
	std::cout << "the oldest return in getOldest: " << oldestMemberUserNick << std::endl;
	return oldestMemberUserNick;
}

void	Channel::setOldestMemberUserAsOperator(){
	channelUsersIt it;
	std::string oldestUserNick = getOldestMemberUser();
	std::cout << "the oldest user is: " << oldestUserNick << std::endl;
	it = _channelUsers.find(oldestUserNick);
	if (it != _channelUsers.end())
        it->second.prefix = "@";
}

void Channel::removeUserAsOperator(std::string nickname){
	channelUsersIt it;
	it = _channelUsers.find(nickname);
    if (it != _channelUsers.end())
        it->second.prefix = "+";
}

void Channel::setMemberLimit(const std::string& limit) {
	std::stringstream ss;
	int limitValue;
	ss << limit;
	ss >> limitValue;
	if (!ss.fail() && limitValue > 0 && limitValue <= 4096)
		_memberLimit = limitValue;
	else if (limitValue <= 0)
		_memberLimit = 1;
	else if (limitValue > 4096)
		_memberLimit = 4096;
}

void Channel::updateNickname(std::string oldNick, std::string newNick){
	channelUsersIt it;
	it = _channelUsers.find(oldNick);
    if (it != _channelUsers.end()){
		user keepUser = it->second;
		_channelUsers.erase(it);
		_channelUsers[newNick] = keepUser;
	}
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

std::string	Channel::getMode(){
	std::string	res = "";

	if (hasMode('t') || hasMode('k') || hasMode('o') || hasMode('l') || hasMode('i'))
		res = "+";
	if (hasMode('t'))
		res += "t";
	if (hasMode('k'))
		res += "k";
	if (hasMode('o'))
		res += "o";
	if (hasMode('i'))
		res += "i";
	if (hasMode('l')){
		res += "l";
	}
	if (hasMode('k'))
		res += " " + getPassword();
	if (hasMode('l')){
		std::stringstream ss;
		std::string limit;
		ss << _memberLimit;
		ss >> limit;
		res += " " + limit;
	}

	return (res);
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
void	Channel::setTopic(std::string newTopic, std::string nickname){
	_topic = newTopic;
	_topicCreationTime = getCurrentTime();
	_topicCreatedBy = nickname;
}

/*
	Returns the topic of the channel.

*/
std::string	Channel::getTopic(){
        return _topic;
}

std::string Channel::getTimeCreationTopic(){
	return _topicCreationTime;
}

std::string Channel::getNickCreationTopic(){
	return _topicCreatedBy;
}

/* --------------- broadcast ---------------- */
void 	Channel::broadcast(std::string message, Client client){
	channelUsersIt it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++){
		if (it->first != *(client.getNick()))
			it->second.client->reply(message.c_str());
	}
}

void Channel::broadcastSenderIncluded(std::string message){
	channelUsersIt it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++){
		it->second.client->reply(message.c_str());
	}
}

/* --------------- utils ---------------- */

std::string Channel::getCurrentTime(){
	time_t current = time(0);
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", localtime(&current));
	return buffer;
}

time_t Channel::getCurrentTimeT(){
	time_t current = time(0);

	return current;
}
