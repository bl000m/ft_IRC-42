#include "Server.hpp"
#include "Client.hpp"

void	Server::send(Client const &client, std::string const &mess)
{
	write(client.getSock(), mess.c_str, mess.size());
}

void	Server::pass(Client const &client, Message const &mess)
{
	std::string	reply;
	
	if (client.isRegist() || client.getNick())
	{
		reply = ":localhost" + ERR_ALREADYREGISTERED + *(client.getNick())
				+ " :you may not reregister\r\n";
		this->send(client, reply);
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		reply = ":localhost" + ERR_NEEDMOREPARAMS + *(client.getNick())
				+ " :Not enough parameters\r\n";
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
	if (client.isRegist())
	{
		//check collision
		//broadcast the change to other user
	}
	else if (client.getPass())
	{
		//check collision
	}
	else
	{
		//unknown command
	}
}

Server::user(Client const &client, Message const &mess)
{

}
