#include "Server.hpp"

void	Server::motd(Client &client, Message const &mess)
{
	(void) client;
	(void) mess;
	reply(client,  ERR_NOMOTD, "no MOTD", NULL);
}

void	Server::wallops(Client &client, Message const &mess)
{
	client_map::const_iterator	i;
	std::string					note;
	
	if (mess.getParamNum() < 1)
	{
		reply(client, ERR_NEEDMOREPARAMS, "WALLOPS", ":Not enough parammeter");
		return ;
	}
	if (client.isServerOp())
	{
		reply(client, ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator", NULL);
		return ;
	}
	note = ":" + client.getFullName() + " WALLOPS "
			+ *(mess.getParam()[0]);
	for (i = _clients.begin(); i != _clients.end(); i++)
	{
		if (i->second.getWallop())
			send(i->second->getSock(), note.c_str(), note.size(), 0);
	}
}

//who
//kill