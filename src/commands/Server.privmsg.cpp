#include "Server.hpp"

void	Server::privmsg(Client &client, Message const &mess)
{
	std::vector<std::string>					target;
	std::vector<std::string>::const_iterator	i;

	if (mess.getParamNum() < 1)
	{
		this->reply(client, ERR_NORECIPIENT, ":No recipient given PRIMSG", NULL);
		return ;
	}
	if (mess.getParamNum() < 2)
	{
		this->reply(client, ERR_NOTEXTTOSEND, ":No text to send", NULL);
		return ;
	}
	target = getTarget(mess.getParam()[0]);
	for (i = target.begin(); i != target.end(); i++)
	{
		if ((*i)[0] == '&' || (*i)[0] == '#')
			;//
		else if (!sendToNick(client, mess, *i))
			this->reply(client, ERR_NOSUCHNICK, i->c_str(), ":No such nick");
	}
}

void	Server::notice(Client &client, Message const &mess)
{
	std::vector<std::string>					target;
	std::vector<std::string>::const_iterator	i;

	if (mess.getParamNum() < 1)
	{
		return ;
	}
	if (mess.getParamNum() < 2)
	{
		return ;
	}
	target = getTarget(mess.getParam()[0]);
	for (i = target.begin(); i != target.end(); i++)
	{
		if ((*i)[0] == '&' || (*i)[0] == '#')
			;//to channel
		else if (!sendToNick(client, mess, *i))
		{
			;
		}
	}
}

std::vector<std::string>	Server::getTarget(std::string const &str)
{
	std::vector<std::string>	temp;
	std::string::size_type		head;
	std::string::size_type		tail;
	std::string					target;

	head = 0;
	while (head != std::string::npos)
	{
		if (str[head] == ',')
			head++;
		tail = str.find(",", head);
		target = str.substr(head, tail);
		temp.push_back(target);
		head = tail;
	}
	return (temp);
}

bool	Server::sendToNick(Client &client, Message const &mess, std::string const &nick)
{
	Client			*target;
	std::string		note;

	target = getClient(nick);
	if (!target)
	{
		return (false);
	}
	note = ":" + client.getFullName() + " " + *(mess.getCommand()) + " " + *(target->getNick()) + " :" + mess.getParam()[1] + "\r\n";
	send(target->getSock(), note.c_str(), note.size(), 0);
	return (true);
}

void	Server::sendToChan(Client &client, Message const &mess, std::string const &name)
{
	Channel		*chan;
	std::string	note;
	bool		response;
	
	response = true;
	if (*mess.getCommand() == "NOTICE")
		response = false;
	chan = getChan(name);
	if (chan == NULL)
	{
		if (response)
			reply(client, ERR_NOSUCHCHANNEL, ":NO suck channel");
		return ;
	}
	if (chan->hasMode('n') && )
	
	
}
