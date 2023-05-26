#include "Server.hpp"

void	Server::execMessage(Client &client, Message const &mess)
{
	int					num;
	std::string const	*cmd;
	std::string			reply;

	reply = ":localhost unknown :command not found\r\n";
	cmd = mess.getCommand();
	if (!cmd)
		return ;
	num = getCmdNum(*(mess.getCommand()));
	if (num == -1)
	{
		this->reply(client, reply);
		return ;
	}
	if (!client.isRegist() && !(*cmd == "PASS" || *cmd == "NICK" || *cmd == "USER"))
	{
		this->reply(client, reply);
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

	if (client.isRegist() || client.getNick())
	{
		reply.append(":localhost").append(ERR_ALREADYREGISTERED).append(*(client.getNick())).append(" :you may not reregister\r\n");
		this->reply(client, reply);
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		reply.append(":localhost").append(ERR_NEEDMOREPARAMS).append("unknown :Not enough parameters\r\n");
		this->reply(client, reply);
		return ;
	}
	if (mess.getParam().front() != this->_password)
	{
		reply.append(":localhost").append(ERR_PASSWDMISMATCH).append("unknown :wrong password\r\n");
		this->reply(client, reply);
		client.setPass(false);
		return ;
	}
	client.setPass(true);
}

void	Server::nick(Client &client, Message const &mess)
{
	/*test*/
	(void) client;
	(void) mess;
	std::cout << "nick" << std::endl;
	return ;

	// std::string	nick;
	// std::string	reply;

	// if (!client.getPass())
	// {
	// 	return ; //ignore
	// }
	// if (mess.getParamNum() < 1)
	// {
	// 	reply = ":localhost" + ERR_NONICKNAMEGIVEN + *(client.getNick())
	// 			+ " :No nickname given\r\n";
	// 	this->send(client, reply);
	// 	return ;
	// }
	// nick = mess.getParam().front();
	// for (map::const_iterator i = _clients.begin(); i != _clients.end(); i++)
	// {
	// 	if ((*i).getNick() && *((*i),getNick()) == nick)
	// 	{
	// 		reply = ":localhost" + ERR_NEEDMOREPARAMS + *(client.getNick())
	// 			+ " :Not enough parameters\r\n"; 
	// 		//nick name in use
	// 		//return 
	// 	}
	// }
	// if (client.isRegist())
	// {
	// 	//update
	// 	//broadcast the change to other user
	// }
	// else if (client.getPass())
	// {
	// 	//update
	// }
}

void	Server::user(Client &client, Message const &mess)
{
	/*test*/
	(void) client;
	(void) mess;
	std::cout << "user" << std::endl;
	return ;
}
