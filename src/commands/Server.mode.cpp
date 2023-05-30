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
		//mode_user
	}
	else
	{
		;//mode channel
	}
}

/*	helper	*/
void	mode_user(Client &client, Message const &mess, std::string target)
{
	std::string	mode;
	
	if (!nick_in_use(target))
	{
		reply(client,  ERR_NOSUCHNICK, target, ":No such nick");
		return ;
	}
	if (*client.getNick() != target)
	{
		reply(client,  ERR_USERSDONTMATCH, ":Cant change mode for other user", NULL);
		return ;
	}
	if (mess.getParamNum() < 2)
	{
		 //RPL_UMODEIS : return current mode of the target
		 return ;
	}
	mode = mess.getParam()[1];
	//set mode
	//reply with MODE message
	// ERR_UMODEUNKNOWNFLAG if there's unknown mode

}