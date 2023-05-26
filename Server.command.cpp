#include "Server.hpp"
#include "Client.hpp"

void	Server::send(Client const &client, std::string const &mess)
{
	write(client.getSock(), mess.c_str, mess.size());
}

void	Server::pass(Client const &client, Message const &mess)
{
	std::string	reply;
	
	if (mess.getParamNum() < 1)
	{
		reply = ":localhost" + ERR_NEEDMOREPARAMS + *(client.getNick())
				+ " :Not enough parameters\r\n";
		this->send(client, reply);
		return ;
	}
	if (client.isRegist() || client.getNick())
	{
		reply = ":localhost" + ERR_ALREADYREGISTERED + *(client.getNick())
				+ " :you may not reregister\r\n";
		this->send(client, reply);
		return ;
	}
	if (mess.getParam().front() != this->_password)
	{
		reply = ":localhost" + ERR_PASSWDMISMATCH + *(client.getNick())
				+ " :Password incorrect\r\n";
		this->send(client, reply);
		client.setPass(false);
		return ;
	}
	client.setPass(true);
}

Server::nick(Client const &client, Message const &mess)
{	
	std::string	nick;
	std::string	reply;
	
	if (!client.getPass())
	{
		return ; //ignore
	}
	if (mess.getParamNum() < 1)
	{
		reply = ":localhost" + ERR_NONICKNAMEGIVEN + *(client.getNick())
				+ " :No nickname given\r\n";
		this->send(client, reply);
		return ;
	}
	nick = mess.getParam().front();
	for (map::const_iterator i = _clients.begin(); i != _clients.end(); i++)
	{
		if ((*i).getNick() && *((*i),getNick()) == nick)
		{
			reply = ":localhost" + ERR_NEEDMOREPARAMS + *(client.getNick())
				+ " :Not enough parameters\r\n"; 
			//nick name in use
			//return 
		}
	}
	if (client.isRegist())
	{
		//update
		//broadcast the change to other user
	}
	else if (client.getPass())
	{
		//update
	}
}

Server::user(Client const &client, Message const &mess)
{

}
