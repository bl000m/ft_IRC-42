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
	std::string	mode;
	Channel *channel = this->getChannel(target);

	 if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, target.c_str(), ":No such channel");
        return;
    }

	std::cout << "RETURN OF GETMODE: " << channel->getMode() << std::endl;
	if (mess.getParamNum() < 2){
		reply(client,  RPL_CHANNELMODEIS , target.c_str(), channel->getMode().c_str());
		return ;
	}

	if (!channel->isUserOperator(*(client.getNick()))) {
        this->reply(client,  ERR_CHANOPRIVSNEEDED, target.c_str(), ":You're not channel operator");
		return;
	}

	mode = mess.getParam()[1];
	for (int i = 2; i < mess.getParamNum(); i++)
	{
		mode += mess.getParam()[i];
	}
	std::cout << "this is MODE: " << mode << std::endl;
	if (setMode(mode, channel, client))
		reply(client, ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag", NULL);
}


bool Server::setMode(std::string mode, Channel* channel, Client& client)
{
    std::string::size_type i;
    bool unknown = false;
    bool op = true;

    if (mode.size() < 1 || (mode[0] != '+' && mode[0] != '-'))
        return false;

    for (i = 0; i < mode.size(); i++)
    {
        switch (mode[i])
        {
            case '+':
                op = true;
                break;
            case '-':
                op = false;
                break;
            case 'i':
                handleIMode(op, channel, client);
                break;
            case 't':
                handleTMode(op, channel, client);
                break;
            case 'o':
                unknown = handleOMode(op, mode, i, channel, client, 'o');
                break;
            case 'k':
                handleKMode(op, mode, i, channel, client, 'k');
                break;
            case 'l':
                handleLMode(op, mode, i, channel, client, 'l');
                break;
            default:
                unknown = true;
        }
    }

    return unknown;
}

void Server::handleIMode(bool op, Channel* channel, Client& client)
{
    if (op)
        channel->addMode('i');
    else
        channel->removeMode('i');

    std::string modeStr = op ? "+i" : "-i";
    std::string message = buildModeMessage(channel, client, modeStr);
    channel->broadcastSenderIncluded(message);
}

void Server::handleTMode(bool op, Channel* channel, Client& client)
{
    if (op)
        channel->addMode('t');
    else
        channel->removeMode('t');

    std::string modeStr = op ? "+t" : "-t";
    std::string message = buildModeMessage(channel, client, modeStr);
    channel->broadcastSenderIncluded(message);
}

bool Server::handleOMode(bool op, const std::string& mode, std::string::size_type& i, Channel* channel, Client& client, char option)
{
    std::string nickname = extractParameter(mode, i, '+', '-', option);

    if (nickname.empty())
        return true; // No nickname provided, mark as unknown

    if (!channel->isUserInChannel(nickname))
    {
        reply(client, ERR_NOSUCHNICK, nickname.c_str(), ":No such nick");
        return true;
    }

    if (op)
    {
        channel->setUserAsOperator(nickname);
    }
    else
    {
        channel->removeUserAsOperator(nickname);
    }

    std::string modeStr = op ? "+o" : "-o";
    std::string message = buildModeMessage(channel, client, modeStr + " " + nickname);
    channel->broadcastSenderIncluded(message);

    return false;
}

void Server::handleKMode(bool op, const std::string& mode, std::string::size_type& i, Channel* channel, Client& client, char option)
{
    std::string password = extractParameter(mode, i, '+', '-', option);

    if (op)
    {
        if (!password.empty())
        {
            channel->setPassword(password);
			channel->addMode('k');
            std::string modeStr = "+k " + password;
            std::string message = buildModeMessage(channel, client, modeStr);
            channel->broadcastSenderIncluded(message);
        }
    }
    else
    {
        channel->removeMode('k');
        std::string modeStr = "-k";
        std::string message = buildModeMessage(channel, client, modeStr);
        channel->broadcastSenderIncluded(message);
    }
}

void Server::handleLMode(bool op, const std::string& mode, std::string::size_type& i, Channel* channel, Client& client, char option)
{
    std::string limit = extractParameter(mode, i, '+', '-', option);

    if (op)
    {
        if (!limit.empty())
        {
            channel->setMemberLimit(limit);
			channel->addMode('l');
            std::string modeStr = "+l " + limit;
            std::string message = buildModeMessage(channel, client, modeStr);
            channel->broadcastSenderIncluded(message);
        }
    }
    else
    {
        channel->removeMode('l');
        std::string modeStr = "-l";
        std::string message = buildModeMessage(channel, client, modeStr);
        channel->broadcastSenderIncluded(message);
    }
}

std::string Server::buildModeMessage(Channel* channel, const Client& client, const std::string& mode)
{
    std::string message = ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost"
                        + " MODE " + channel->getName() + " " + mode + "\r\n";
    return message;
}

std::string Server::extractParameter(const std::string& mode, std::string::size_type& i, char op1, char op2, char option)
{
    std::string parameter;

    while (i < mode.size() && mode[i] != op1 && mode[i] != op2)
    {
        if (mode[i] == ' ' || mode[i] == option)
            i++;
        parameter += mode[i];
        i++;
    }

    return parameter;
}

// bool	Server::setMode(std::string mode, Channel *channel, Client &client)
// {
// 	std::string::size_type	i;
// 	bool	unknown = false;
// 	bool	op = true;
// 	std::string password;
// 	std::string	limit;
// 	std::string nickname;
// 	std::string message;

// 	if (mode.size() < 1 || (mode[0] != '+' && mode[0] != '-'))
// 		return (false);
// 	for (i = 0; i < mode.size(); i++)
// 	{
// 		switch(static_cast<int>(mode[i]))
// 		{
// 			case '+':
// 				op = true;
// 				break ;
// 			case '-':
// 				op = false;
// 				break ;
// 			case 'i':
// 				if (op == true){
// 					channel->addMode('i');
// 					message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 							+ " MODE " + channel->getName() + " " + "+i" + "\r\n";
// 					channel->broadcastSenderIncluded(message);
// 				}
// 				else{
// 					channel->removeMode('i');
// 					message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 							+ " MODE " + channel->getName() + " " + "-i" + "\r\n";
// 					channel->broadcastSenderIncluded(message);
// 				}
// 				break ;
// 			case 't':
// 				if (op == true){
// 					channel->addMode('t');
// 					message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 							+ " MODE " + channel->getName() + " " + "+t" + "\r\n";
// 					channel->broadcastSenderIncluded(message);
// 				}
// 				else{
// 					channel->removeMode('t');
// 					message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 							+ " MODE " + channel->getName() + " " + "-t" + "\r\n";
// 					channel->broadcastSenderIncluded(message);
// 				}
// 				break ;
// 			case 'o':
// 				while (i < mode.size() && mode[i] != '+' && mode[i] != '-'){
// 					if (mode[i] == ' ' || mode[i] == 'o')
// 						i++;
// 					nickname += mode[i];
// 					i++;
// 				}
// 				std::cout << "nickname in case 'o'" << nickname << std::endl;
// 				if (!channel->isUserInChannel(nickname)){
// 					reply(client,  ERR_NOSUCHNICK, nickname.c_str(), ":No such nick");
// 					unknown = true;
// 				}
// 				if (!nickname.empty() && channel->isUserInChannel(nickname)){
// 					if (op == false){
// 						channel->removeUserAsOperator(nickname);
// 						message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 						+ " MODE " + channel->getName() + " " + "-o" + " " + nickname + "\r\n";
// 						channel->broadcastSenderIncluded(message);
// 					}
// 					else{
// 						channel->setUserAsOperator(nickname);
// 						message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 						+ " MODE " + channel->getName() + " " + "+o" + " " + nickname + "\r\n";
// 						channel->broadcastSenderIncluded(message);
// 					}
// 				}
//                 else
//                     unknown = true; // No nickname provided, mark as unknown
//                 break;
// 			case 'k':
// 				if (op == true){
// 					while (i < mode.size() && mode[i] != '+' && mode[i] != '-'){
// 						if (mode[i] == ' ' || mode[i] == 'k')
// 							i++;
// 						password += mode[i];
// 						i++;
// 					}
// 					if (!password.empty()){
// 							channel->setPassword(password);
// 							message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 								+ " MODE " + channel->getName() + " " + "+k" + " " + password + "\r\n";
// 							channel->broadcastSenderIncluded(message);
// 					}
// 				}
// 				else{
// 					channel->removeMode('k');
// 					message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 						+ " MODE " + channel->getName() + " " + "-k" + "\r\n";
// 					channel->broadcastSenderIncluded(message);
// 				}
// 				break ;
// 			case 'l':
// 				if (op == true){
// 					while (i < mode.size() && mode[i] != '+' && mode[i] != '-'){
// 						if (mode[i] == ' ' || mode[i] == 'l')
// 							i++;
// 						limit += mode[i];
// 						i++;
// 					}
// 					if (!limit.empty()){
// 					channel->setMemberLimit(limit);
// 					message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 							+ " MODE " + channel->getName() + " " + "+l" + " " + limit + "\r\n";
// 					channel->broadcastSenderIncluded(message);
// 					}
// 				}
// 				else{
// 					channel->removeMode('l');
// 					message =  ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@localhost" \
// 							+ " MODE " + channel->getName() + " " + "-l" + "\r\n";
// 					channel->broadcastSenderIncluded(message);
// 				}
// 				break ;
// 			default:
// 				unknown = true;
// 		}
// 	}
// 	return (unknown);
// }
