#include "Server.hpp"

void	Server::motd(Client &client, Message const &mess)
{
	(void) client;
	(void) mess;
	client.reply(ERR_NOMOTD, ":no MOTD", NULL);
}

void	Server::wallops(Client &client, Message const &mess)
{
	client_map::iterator	i;
	std::string				note;
	
	if (mess.getParamNum() < 1)
	{
		client.reply(ERR_NEEDMOREPARAMS, "WALLOPS", ":Not enough parammeter");
		return ;
	}
	if (!client.isServerOp())
	{
		client.reply(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator", NULL);
		return ;
	}
	note = ":" + client.getFullName() + " WALLOPS :"
			+ mess.getParam()[0];
	for (i = _clients.begin(); i != _clients.end(); i++)
	{
		if (i->second.getWallop())
			i->second.reply(note.c_str());
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
		return ;
	}
	if (!client.isServerOp())
	{
		client.reply(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator", NULL);
		return ;
	}
	victim = getClient(mess.getParam()[0]);
	if (!victim)
	{
		client.reply(ERR_NOSUCHNICK, mess.getParam()[0].c_str(), ":No such nick");
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
	rmClient(*victim);
}
