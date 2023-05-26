#include "Server.hpp"

void	Server::execMessage(Client &client, Message const &mess)
{
	int					num;
	std::string const	*cmd;

	cmd = mess.getCommand();
	if (!cmd)
		return ;
	num = getCmdNum(*(mess.getCommand()));
	if (num == -1)
	{
		note = note + ERR_UNKNOWNCOMMAND + nick + " " + *cmd + " :Unknown command\r\n";
		this->reply(client, note);
		return ;
	}
	if (!client.isRegist()
			&& !(*cmd == "PASS" || *cmd == "NICK" || *cmd == "USER"))
	{
		note = note + ERR_UNKNOWNCOMMAND + nick + " " + *cmd + " :Unknown command\r\n";
		this->reply(client, note);
		return ;
	}
	(this->*_cmd[num])(client, mess);
}

int	Server::getCmdNum(std::string const &cmd)
{
	if (cmd == "PASS")
		return (PASS);
	if (cmd == "NICK")
		return (NICK);
	if (cmd == "USER")
		return (USER);
	return (-1);
}

void	Server::reply(Client const &client, std::string const &mess)
{
	send(client.getSock(), mess.c_str(), mess.size(), 0);
}

void	Server::pass(Client &client, Message const &mess)
{	
	std::string	note = ":localhost";
	std::string	nick = "unknown";

	if (client.getNick())
		nick = *(client.getNick());
	if (client.isRegist() || client.getNick())
	{
		note = note + ERR_ALREADYREGISTERED + nick + " :you may not reregister\r\n";
		this->reply(client, note);
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		note = note + ERR_NEEDMOREPARAMS + "unknown PASS :Not enough parameters\r\n";
		this->reply(client, note);
		return ;
	}
	if (mess.getParam().front() != this->_password)
	{
		note = note + ERR_PASSWDMISMATCH + "unknown :wrong password\r\n";
		this->reply(client, note);
		client.setPass(false);
		return ;
	}
	client.setPass(true);
}

void	Server::nick(Client &client, Message const &mess)
{
	std::string	new_nick;
	std::string	nick = "unknown";
	std::string	note = ":localhost";
	
	if (client.getNick())
		nick = *(client.getNick());
	if (client.getPass() == false)
	{
		note = note + ERR_UNKNOWNCOMMAND + nick + " NICK :Unknown command\r\n";
		this->reply(client, note);		return ;
	}
	if (mess.getParamNum() < 1)
	{
		note = note + ERR_NONICKNAMEGIVEN + nick + " :No nickname given\r\n";
		this->reply(client, note);
		return ;
	}
	new_nick = mess.getParam().front();
	//check nick name valid
	for (map::const_iterator i = _clients.begin(); i != _clients.end(); i++)
	{
		if ((i->second).getNick() && *((i->second).getNick()) == nick)
		{
			note = note + ERR_NICKNAMEINUSE + nick + " :Nickname is already in use\r\n";
			this->reply(client, note);
			return ;
		}
	}
	client.setNick(new_nick);
	if (client.isRegist())
	{	
		//broadcast the change to other user
	}
}

void	Server::user(Client &client, Message const &mess)
{
	std::string	nick = "unknown";
	std::string	note = ":localhost";

	if (client.getNick())
		nick = *(client.getNock());
	if (client.isRegist())
	{
		note = note + ERR_ALREADYREGISTERED + ":You may not reregister\r\n";
		this->reply(client, note);
		return ;
	}
	if (client.getNick() == NULL)
	{
		note = note + ERR_UNKNOWNCOMMAND + nick + " USER :Unknown command\r\n";
		this->reply(client, note);
		return ;
	}
	if (mess.getParamNum() < 4)
	{
		note = note + ERR_NEEDMOREPARAMS + nick + " USER ::Not enough parameters\r\n";
		this->reply(client, note);
		return ;
	}
	//set user and host name
	//send welcome mess etc
}
