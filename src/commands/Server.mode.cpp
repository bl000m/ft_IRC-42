#include "Server.hpp"

void	Server::mode(Client &client, Message const &mess)
{
	std::string	target;

	if (mess.getParamNum() < 1)
	{
		reply(client,  ERR_NEEDMOREPARAMS, "MODE", ":Not enough parameters");
		return ;
	}
	target = mess.getParam()[0];
	if (target[0] != '#' && target[0] != '&')
	{
		mode_user(client, mess, target);
	}
	else
	{
		mode_channel(client, mess, target);
	}
}

/*	helper	*/
void	Server::mode_user(Client &client, Message const &mess, std::string target)
{
	std::string	mode;

	if (!nick_in_use(target))
	{
		reply(client,  ERR_NOSUCHNICK, target.c_str(), ":No such nick");
		return ;
	}
	if (*client.getNick() != target)
	{
		reply(client,  ERR_USERSDONTMATCH, ":Cant change mode for other user", NULL);
		return ;
	}
	if (mess.getParamNum() < 2)
	{
		reply(client,  RPL_UMODEIS, client.getMode().c_str(), NULL);
		return ;
	}
	mode = mess.getParam()[1];
	if (client.setMode(mode))
		reply(client, ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag", NULL);
	reply(client, "MODE", client.getMode().c_str(), NULL);
}

/**
 CHANNEL MODES:
 - o : channel operator status to specified nickname
 => MODE #channel (+ or -)o username
 - t : topic is locked and can only be changed by channel operator
 => MODE #channel -t
 - k : key (password) protected channel => locks the channel with given password. When joining the channel every user must specify the password to access it
 => MODE #channel -k password
 - l : user limit => only the given number of members can be in the channel. If getUsersCount >= limit no other user is accepted
 => MODE #channel -l user_limit
 - i : invite only => clients must be invited to join the channel when this mode is set
 => MODE #channel -i

 All of them => MODE #channel +o nickname +i +k password +t +l limit_number
 +okitl nickname password num
*/
void	Server::mode_channel(Client &client, Message const &mess, std::string target){
	std::string	modeOptions;
	std::string	modeArgs;
	Channel *channel = this->getChannel(target);
	channelModeListIt it;

	if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, target.c_str(), ":No such channel");
        return;
    }



	//debug
	// for (it = _channelModes.begin(); it != _channelModes.end(); it++){
	// 	std::cout << "option = " << it->first << " => arg = " << it->second << std::endl;
	// }

	// std::cout << "RETURN OF GETMODE: " << channel->getMode() << std::endl;
	if (mess.getParamNum() < 2){
		reply(client,  RPL_CHANNELMODEIS , target.c_str(), channel->getMode().c_str());
		return ;
	}

	modeOptions = mess.getParam()[1];
	//debug
	std::cout << "mode options: " << modeOptions << std::endl;
	if (!parseChannelModes(modeOptions, mess))
		// this->reply(client,  ERR_NEEDMOREPARAMS, target.c_str(), ":Not enough parameters");
		return;

	if (!channel->isUserOperator(*(client.getNick()))) {
        this->reply(client,  ERR_CHANOPRIVSNEEDED, target.c_str(), ":You're not channel operator");
		return;
	}

	setMode(channel, client);
}

bool Server::parseChannelModes(const std::string& modeString, Message const &mess)
{

    char sign;
    std::string option;
  	std::vector<std::string> optionVector;
	  std::string key;

	  if (modeString.empty() || (modeString[0] != '+' && modeString[0] != '-')){
        // std::cout << "IN FIRST CHECK of PARSE" << std::endl;
        return false;
    }
    // std::cout << "modeString size = " << modeString.size() << std::endl;
    for (size_t i = 0; i < modeString.size(); i++)
    {
        if (modeString[i] == '+' || modeString[i] == '-')
        {
            sign = modeString[i];
        }
        else if (modeString[i] == 'o' || modeString[i] == 't' || modeString[i] == 'i'
                || modeString[i] == 'k' || modeString[i] == 'l')
        {
            key += sign;
            key += modeString[i];
            // std::cout << "key = " << key << std::endl;
            optionVector.push_back(key);
            // std::cout << "Value of the last item: " << optionVector.back() << std::endl;
            key.clear();
        }
    }
    std::vector<std::string>::iterator itVec;
    itVec = optionVector.begin();
    // std::cout << "out: *itVec = " << *itVec << std::endl;
    // std::cout << "n. pqrqms" << mess.getParamNum() << std::endl;
    if (mess.getParamNum() == 2)
    {
      if (*itVec == "+i" || *itVec == "-i" || *itVec == "+t"
          || *itVec == "-t" || *itVec == "-k" || *itVec == "+l")
      {
          // std::cout << "in: *itVec = " << *itVec << std::endl;
          _channelModes.insert(std::make_pair(*itVec, ""));
      }
    }

    int i = 2;
    while (i < mess.getParamNum())
    {
      for (itVec = optionVector.begin(); itVec != optionVector.end(); itVec++)
      {
        if ((*itVec == "+o" || *itVec == "+k" || *itVec == "+l"
          || *itVec == "-o") && !mess.getParam()[i].empty())
          {
            _channelModes.insert(std::make_pair(*itVec, mess.getParam()[i]));
            i++;
          }
        else if ((*itVec == "+o" || *itVec == "+k" || *itVec == "+l"
          || *itVec == "-o") && mess.getParam()[i].empty())
            return false;
        else{
          std::cout << "*itVec = " << *itVec << std::endl;
            _channelModes.insert(std::make_pair(*itVec, ""));
            // continue;
        }
      }
    }
    return true;
}

bool Server::setMode(Channel* channel, Client& client)
{
	channelModeListIt it;
  bool unknown = false;

	for (it = _channelModes.begin(); it != _channelModes.end(); it++)
  {
    // std::cout << "in setMode, it->first = " << it->first << std::endl;
		handleIMode(it, channel, client);
    handleTMode(it, channel, client);
    unknown = handleOMode(it, channel, client);
    handleKMode(it, channel, client);
    handleLMode(it, channel, client);
	}
  std::map<std::string, std::string> emptyMap;
    _channelModes.swap(emptyMap);
  return unknown;
}

void Server::handleIMode(channelModeListIt it, Channel* channel, Client& client)
{
    if (it->first == "+i")
    {
      // std::cout << "IN handlemode I" << std::endl;
      channel->addMode('i');
      std::string message = buildModeMessage(channel, client, it->first);
      channel->broadcastSenderIncluded(message);
    }
    else if (it->first == "-i")
    {
      channel->removeMode('i');
      std::string message = buildModeMessage(channel, client, it->first);
      channel->broadcastSenderIncluded(message);
    }
}

void Server::handleTMode(channelModeListIt it, Channel* channel, Client& client)
{
    if (it->first == "+t")
    {
      channel->addMode('t');
      std::string message = buildModeMessage(channel, client, it->first);
      channel->broadcastSenderIncluded(message);
    }
    else if (it->first == "-t")
    {
      channel->removeMode('t');
      std::string message = buildModeMessage(channel, client, it->first);
      channel->broadcastSenderIncluded(message);
    }
}

bool Server::handleOMode(channelModeListIt it, Channel* channel, Client& client)
{
    if (it->first == "+o")
    {
        if (!channel->isUserInChannel(it->second))
        {
            reply(client, ERR_NOSUCHNICK, it->second.c_str(), ":No such nick");
            return true;
        }
        channel->setUserAsOperator(it->second);
        std::string message = buildModeMessage(channel, client, it->first + " " + it->second);
        channel->broadcastSenderIncluded(message);
    }
    else if (it->first == "-o")
     {
        if (!channel->isUserInChannel(it->second))
        {
            reply(client, ERR_NOSUCHNICK, it->second.c_str(), ":No such nick");
            return true;
        }
        channel->removeUserAsOperator(it->second);
        std::string message = buildModeMessage(channel, client, it->first + " " + it->second);
        channel->broadcastSenderIncluded(message);
    }
    return false;
}

void Server::handleKMode(channelModeListIt it, Channel* channel, Client& client)
{
    if (it->first == "+k")
    {
        channel->setPassword(it->second);
        channel->addMode('k');
        std::string message = buildModeMessage(channel, client, it->first + " " + it->second);
        channel->broadcastSenderIncluded(message);
    }
    else if (it->first == "-k")
    {
        channel->removeMode('k');
        std::string message = buildModeMessage(channel, client, it->first);
        channel->broadcastSenderIncluded(message);
    }
}

void Server::handleLMode(channelModeListIt it, Channel* channel, Client& client)
{
    if (it->first == "+l")
    {
		std::cout << "second in handleLmode: " << it->second << std::endl;
        channel->setMemberLimit(it->second);
        channel->addMode('l');
        std::string message = buildModeMessage(channel, client, it->first + " " + it->second);
        channel->broadcastSenderIncluded(message);
    }
    else if (it->first == "-l")
    {
        channel->removeMode('l');
        std::string message = buildModeMessage(channel, client, it->first);
        channel->broadcastSenderIncluded(message);
    }
}

std::string Server::buildModeMessage(Channel* channel, const Client& client, const std::string& mode)
{
    std::string message = ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost"
                        + " MODE " + channel->getName() + " " + mode + "\r\n";
    return message;
}
