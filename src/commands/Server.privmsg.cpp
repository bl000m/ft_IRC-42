#include "Server.hpp"

static std::vector<std::string>	getTarget(std::string const &str);

void	Server::privmsg(Client &client, Message const &mess)
{
	std::vector<std::string>					target;
	std::vector<std::string>::const_iterator	i;

	if (mess.getParamNum() < 1)
	{
		client.reply(ERR_NORECIPIENT, ":No recipient given PRIMSG", NULL);
		return ;
	}
	if (mess.getParamNum() < 2)
	{
		client.reply(ERR_NOTEXTTOSEND, ":No text to send", NULL);
		return ;
	}
	target = getTarget(mess.getParam()[0]);
	for (i = target.begin(); i != target.end(); i++)
	{
		if ((*i)[0] == '&' || (*i)[0] == '#')
			sendToChan(client, mess, *i);
		else if (!sendToNick(client, mess, *i))
			client.reply(ERR_NOSUCHNICK, i->c_str(), ":No such nick");
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
			sendToChan(client, mess, *i);
		else
			sendToNick(client, mess, *i);
	}
}

static std::vector<std::string>	getTarget(std::string const &str)
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
	note = ":" + client.getFullName() + " " + *(mess.getCommand()) + " " + *(target->getNick()) + " :" + mess.getParam()[1];
	target->reply(note.c_str());
	//check away
	if (*mess.getCommand() == "PRIVMSG" && !target->getAway().empty())
	{
		client.reply(RPL_AWAY, nick.c_str(), target->getAway().c_str());
	}
	return (true);
}

void	Server::sendToChan(Client &client, Message const &mess, std::string const &name)
{
	bool		response;
	Channel		*chan;
	std::string	note;
	
	response = true;
	if (*mess.getCommand() == "NOTICE")
		response = false;
	chan = getChan(name);
	if (chan == NULL)
	{
		if (response)
			client.reply(ERR_NOSUCHCHANNEL, ":NO such channel", NULL);
		return ;
	}
	if (chan->hasMode('n') && chan->isUserInChannel(*client.getNick()) == false)
	{
		if (response)
			client.reply(ERR_CANNOTSENDTOCHAN, ":cannot send to channel", NULL);
		return ;
	}
	note = ":" + client.getFullName() + " " + *mess.getCommand() \
			+ " " + name + " :" + mess.getParam()[1] + "\r\n";
	chan->broadcast(note, client);
}
