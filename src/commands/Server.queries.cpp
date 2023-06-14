#include "Server.hpp"

void	Server::motd(Client &client, Message const &mess)
{
	(void) client;
	(void) mess;
	client.reply(ERR_NOMOTD, ":no MOTD", NULL);
	// reply(client,  ERR_NOMOTD, ":no MOTD", NULL);
}

void	Server::wallops(Client &client, Message const &mess)
{
	client_map::iterator	i;
	std::string				note;
	
	if (mess.getParamNum() < 1)
	{
		client.reply(ERR_NEEDMOREPARAMS, "WALLOPS", ":Not enough parammeter");
		// reply(client, ERR_NEEDMOREPARAMS, "WALLOPS", ":Not enough parammeter");
		return ;
	}
	if (!client.isServerOp())
	{
		client.reply(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator", NULL);
		// reply(client, ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator", NULL);
		return ;
	}
	note = ":" + client.getFullName() + " WALLOPS :"
			+ mess.getParam()[0];
	for (i = _clients.begin(); i != _clients.end(); i++)
	{
		if (i->second.getWallop())
			i->second.reply(note.c_str());
			// send(i->second.getSock(), note.c_str(), note.size(), 0);
	}
}

void	Server::kill(Client &client, Message const &mess)
{
	Client				*victim;
	std::string			note_to_all;
	std::string			note_to_vic;
	channelListIt		i;
	
	if (mess.getParamNum() < 2)
	{
		client.reply(ERR_NEEDMOREPARAMS, "KILL", ":Not enough parameters");
		// reply(client, ERR_NEEDMOREPARAMS, "KILL", ":Not enough parameters");
		return ;
	}
	if (!client.isServerOp())
	{
		client.reply(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator", NULL);
		// reply(client, ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator", NULL);
		return ;
	}
	victim = getClient(mess.getParam()[0]);
	if (!victim)
	{
		client.reply(ERR_NOSUCHNICK, mess.getParam()[0].c_str(), ":No such nick");
		// this->reply(client, ERR_NOSUCHNICK, mess.getParam()[0].c_str(), ":No such nick");
		return ;
	}
	note_to_all = ":" + victim->getFullName() + " QUIT Killed " + *client.getNick();
	for (i = _channels.begin(); i != _channels.end(); i++)
	{
		if (i->second.isUserInChannel(*victim->getNick()))
		{
			i->second.broadcast(note_to_all, *victim);
			i->second.removeChannelUser(*victim->getNick());
		}
	}
	note_to_vic = ":localhost ERROR :killed " + client.getFullName() + " " + mess.getParam()[1];
	victim->reply(note_to_all.c_str());
	victim->reply(note_to_vic.c_str());
	// send(victim->getSock(), note_to_all.c_str(), note_to_all.size(), 0);
	// send(victim->getSock(), note_to_vic.c_str(), note_to_vic.size(), 0);
	rmClient(*victim);
}


/*
	string matching with wild card
	mask contain * or ?
*/
bool	isMatch(std::string name, std::string mask)
{
	int	i;
	int	j;
	int	match;
	int	wildcard;

	i = 0;
	j = 0;
	match = 0;
	wildcard = -1;
	while (i < name.size())
	{
		if (j < mask.size() && (mask[j] == '?' || mask[j] == name[i]))
		{
			i++;
			j++;
		}
		else if (j < mask.size() && mask[j] == '*')
		{
			wildcard = j;
			j++;
			match = i;
		}
		else if (wildcard != -1)
		{
			j = wildcard + 1;
			match++;
			i = match;
		}
		else
			return (false);
	}
	for (; j < mask.size(); j++)
	{
		if (mask[j] != '*')
			return (false);
	}
	return (true);
}