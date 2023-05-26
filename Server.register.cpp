#include "Server.hpp"

void	Server::execMessage(Client &client, Message const &mess)
{
	int					num;
	std::string const	*cmd;

	cmd = mess.getCommand();
	if (!cmd)
		return ;
	num = getCmdNum(*cmd);
	if (num == -1)
	{
		this->reply(client, ERR_UNKNOWNCOMMAND, cmd->c_str(), ":Unknown command");
		return ;
	}
	if (!client.isRegist()
			&& !(*cmd == "PASS" || *cmd == "NICK" || *cmd == "USER"))
	{
		this->reply(client, ERR_UNKNOWNCOMMAND, cmd->c_str(), ":Unknown command");
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

void	Server::pass(Client &client, Message const &mess)
{	
	if (client.isRegist() || client.getNick())
	{
		this->reply(client, ERR_ALREADYREGISTERED, ":Unknown command", NULL);
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		this->reply(client,  ERR_NEEDMOREPARAMS, "PASS", ":Not enough parameters");
		return ;
	}
	if (mess.getParam().front() != this->_password)
	{
		this->reply(client,  ERR_PASSWDMISMATCH, ":wrong password", NULL);
		client.setPass(false);
		return ;
	}
	client.setPass(true);
}

void	Server::nick(Client &client, Message const &mess)
{
	std::string	new_nick;
	
	if (client.getPass() == false)
	{
		this->reply(client,  ERR_UNKNOWNCOMMAND, "NICK", ":Unknown command");
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		this->reply(client,  ERR_NONICKNAMEGIVEN, ":No nickname given", NULL);
		return ;
	}
	new_nick = mess.getParam().front();
	//check nick name valid
	if (nick_in_use(new_nick))
	{
		this->reply(client,  ERR_NICKNAMEINUSE, ":Nickname is already in use", NULL);
		return ;
	}
	client.setNick(new_nick);
	if (client.isRegist())
	{	
		//broadcast the change to other user
	}
}

void	Server::user(Client &client, Message const &mess)
{
	std::vector<std::string>	param;

	if (client.isRegist())
	{
		this->reply(client, ERR_ALREADYREGISTERED, ":You may not reregister", NULL);
		return ;
	}
	if (client.getNick() == NULL)
	{
		this->reply(client, ERR_UNKNOWNCOMMAND, "USER", ":Unknown command");
		return ;
	}
	if (mess.getParamNum() < 4)
	{
		this->reply(client, ERR_NEEDMOREPARAMS, "USER", ":Not enough parameters");
		return ;
	}
	param = mess.getParam();
	//check if valid
	client.setUser(param[0]);
	client.setHost(param[3]);
	client.beRegist();
	this->reply(client, RPL_WELCOME, ":Welcome to our network", NULL);
	this->reply(client, RPL_YOURHOST, ":Your host is ft_irc", NULL);
	this->reply(client, RPL_CREATED, ":This version is created yesterday", NULL);
	this->reply(client, RPL_MYINFO, "<servername> <version> <available user modes> <available channel modes>", NULL);
	//send isupport
}

/*	helper	*/

bool	Server::nick_in_use(std::string const &nick) const
{
	for (map::const_iterator i = _clients.begin(); i != _clients.end(); i++)
	{
		if ((i->second).getNick() && *((i->second).getNick()) == nick)
		{
			return (true);
		}
	}
	return (false);
}

/*	
	send to client if the message has the form:
	:localhost command nick para1 para2
	:localhost command nick para1 
	:localhost command nick
	the function simply append all together with space in between
	add \r\n at the end
*/
void	Server::reply(Client const &client, std::string const *cmd, char const *p1, char const *p2)
{
	std::string	nick = " unknown ";
	std::string	note = ":localhost ";

	if (client.getNick())
		nick = *(client.getNick());
	note = note + *cmd + " ";
	note = note + nick + " ";
	if (p1)
		note = note + *p1;
	if (p2)
		note = note + *p2;
	note = note + "\r\n";
	send(client.getSock(), note.c_str(), note.size(), 0);
}

void	Server::reply(Client const &client, char const *numeric, char const *p1, char const *p2)
{
	std::string	nick = " unknown ";
	std::string	note = ":localhost ";

	if (client.getNick())
		nick = *(client.getNick());
	note = note + numeric + " ";
	note = note + nick + " ";
	if (p1)
		note = note + p1 + " ";
	if (p2)
		note = note + p2;
	note = note + "\r\n";
	send(client.getSock(), note.c_str(), note.size(), 0);
}