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
		this->reply(client, ":localhost unknown :command not found\r\n");
		return ;
	}
	if (!client.isRegist()
			&& !(*cmd == "PASS" || *cmd == "NICK" || *cmd == "USER"))
	{
		this->reply(client, ":localhost unknown :command not found\r\n");
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
	std::string	reply;
	std::string	localhost = ":localhost";
	std::string	nick = "unknown";

	if (client.getNick())
		nick = *(client.getNick());
	if (client.isRegist() || client.getNick())
	{
		reply = localhost + ERR_ALREADYREGISTERED + nick + " :you may not reregister\r\n";
		this->reply(client, reply);
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		reply = localhost + ERR_NEEDMOREPARAMS + "unknown :Not enough parameters\r\n";
		this->reply(client, reply);
		return ;
	}
	if (mess.getParam().front() != this->_password)
	{
		reply = localhost + ERR_PASSWDMISMATCH + "unknown :wrong password\r\n";
		this->reply(client, reply);
		client.setPass(false);
		return ;
	}
	client.setPass(true);
}

void	Server::nick(Client &client, Message const &mess)
{
	std::string	reply;
	std::string	new_nick;
	std::string	localhost = ":localhost";
	std::string	nick = "unknown";
	
	if (client.getNick())
		nick = *(client.getNick());
	if (!client.getPass())
	{
		this->reply(client, ":localhost unknown :command not found\r\n");
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		reply = localhost + ERR_NONICKNAMEGIVEN + nick + " :No nickname given\r\n";
		this->reply(client, reply);
		return ;
	}
	new_nick = mess.getParam().front();
	for (map::const_iterator i = _clients.begin(); i != _clients.end(); i++)
	{
		if ((i->second).getNick() && *((i->second).getNick()) == nick)
		{
			reply = localhost + ERR_NICKNAMEINUSE + nick + " :Nickname is already in use\r\n";
			this->reply(client, reply);
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
	/*test*/
	(void) client;
	(void) mess;
	std::cout << "user" << std::endl;
	return ;
}
