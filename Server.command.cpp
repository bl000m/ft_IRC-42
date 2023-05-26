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