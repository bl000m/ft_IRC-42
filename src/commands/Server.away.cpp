#include "Server.hpp"

void	Server::away(Client &client, Message const &mess)
{
	std::string	text;
	
	if (mess.getParamNum() < 1 || mess.getParam()[0].empty())
	{
		client.setAway("");
		client.reply(RPL_UNAWAY, ":You are no longer marked as being away", NULL);
		return ;
	}
	text = mess.getParam()[0];
	client.setAway(text);
	client.reply(RPL_NOWAWAY, ":You have been marked as being away");
}