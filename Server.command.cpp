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
	std::map<std::string, int>::const_iterator	i;

	i = _cmdNum.find(cmd);
	if (i == _cmdNum.end())
		return (-1);
	return (i->second);
}

/*	
	send to client if the message has the form:
	:localhost command nick para1 para2
	:localhost command nick para1 
	:localhost command nick
	the function simply append all together with space in between
	add \r\n at the end
*/
void	Server::reply(Client const &client, char const *cmd, char const *p1, char const *p2)
{
	std::string	nick = " unknown ";
	std::string	note = ":localhost ";

	if (client.getNick())
		nick = *(client.getNick());
	note = note + cmd + " ";
	note = note + nick;
	if (p1)
		note = note + " " + p1;
	if (p2)
		note = note + " " + p2;
	note = note + "\r\n";
	//the 4th para of send use default temporarily
	send(client.getSock(), note.c_str(), note.size(), 0);
}

/*
	A message from source (a client) to all other client
	on the server
*/
void	Server::broadcast(Client const &client, char const *cmd, char const *p1, char const *p2)
{
	std::string	note;
	std::string	src = client.getFullName();
	map const	&clients = this->_clients;

	note = ":" + src;
	if (cmd)
		note = note + " " + cmd;
	if (p1)
		note = note + " " + p1;
	if (p2)
		note = note + " " + p2;
	note += "\r\n";
	for (map::const_iterator i = clients.begin(); i != clients.end(); i++)
	{
		if (i->second.getFullName() == src)
			continue ;
		send(client.getSock(), note.c_str(), note.size(), 0);
	}
	return ;
}

std::map<std::string, int>	Server::cmdNum_init(void)
{
	std::map<std::string, int>	temp;

	temp["PASS"] = 0;
	temp["NICK"] = 1;
	temp["USER"] = 2;

	return (temp);
}

std::map<std::string, int>	Server::_cmdNum = cmdNum_init();
