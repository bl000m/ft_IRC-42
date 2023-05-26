#include "Server.hpp"

void	Server::execMessage(Client const &client, Message const &mess)
{
	int					num;
	std::string const	*cmd;
	
	cmd = mess.getCommand();
	if (!cmd)
		return ;
	num = getCmdNum(*(mess.getCommand()));
	if (num == -1)
	{
		//reply cmd not found
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

// void	Server::reply(Client const &client, std::string const &mess)
// {
// 	write(client.getSock(), mess.c_str, mess.size());
// }

void	Server::pass(Client const &client, Message const &mess)
{
	
	/*test*/
	(void) client;
	(void) mess;
	std::cout << "pass" << std::endl;
	return ;
	
	// std::string	reply;
	// if (mess.getParamNum() < 1)
	// {
	// 	reply = ":localhost" + ERR_NEEDMOREPARAMS + *(client.getNick())
	// 			+ " :Not enough parameters\r\n";
	// 	this->reply(client, reply);
	// 	return ;
	// }
	// if (client.isRegist() || client.getNick())
	// {
	// 	reply = ":localhost" + ERR_ALREADYREGISTERED + *(client.getNick())
	// 			+ " :you may not reregister\r\n";
	// 	this->reply(client, reply);
	// 	return ;
	// }
	// if (mess.getParam().front() != this->_password)
	// {
	// 	reply = ":localhost" + ERR_PASSWDMISMATCH + *(client.getNick())
	// 			+ " :Password incorrect\r\n";
	// 	this->reply(client, reply);
	// 	client.setPass(false);
	// 	return ;
	// }
	// client.setPass(true);
}

void	Server::nick(Client const &client, Message const &mess)
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

void	Server::user(Client const &client, Message const &mess)
{
	/*test*/
	(void) client;
	(void) mess;
	std::cout << "user" << std::endl;
	return ;
}
