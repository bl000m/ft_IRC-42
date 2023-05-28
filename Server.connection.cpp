#include "Server.hpp"

void	Server::pass(Client &client, Message const &mess)
{	
	if (client.isRegist() || client.getNick())
	{
		this->reply(client, ERR_ALREADYREGISTERED, ":You may not reregister", NULL);
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
		this->reply(client, ERR_UNKNOWNCOMMAND, "NICK", ":Unknown command");
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		this->reply(client, ERR_NONICKNAMEGIVEN, ":No nickname given", NULL);
		return ;
	}
	new_nick = mess.getParam().front();
	if (nick_valid(new_nick) == false)
	{
		this->reply(client, ERR_ERRONEUSNICKNAME, new_nick.c_str(), ":Erroneus nickname");
		return ;
	}
	if (nick_in_use(new_nick))
	{
		this->reply(client, ERR_NICKNAMEINUSE, ":Nickname is already in use", NULL);
		return ;
	}
	if (client.isRegist())
		broadcast(client, "NICK", new_nick.c_str(), NULL);
	client.setNick(new_nick);
}

void	Server::user(Client &client, Message const &mess)
{
	std::vector<std::string>	param;

	param = mess.getParam();
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
	if (param.size() < 4 || param[0].size() < 1)
	{
		this->reply(client, ERR_NEEDMOREPARAMS, "USER", ":Not enough parameters");
		return ;
	}
	//trunc user name if required
	client.setUser(param[0]);
	client.setHost(param[3]);
	client.beRegist();
	this->welcome_mess(client);
}

void	Server::quit(Client &client, Message const &mess)
{
	client_map::iterator			i;
	std::vector<pollfd>::iterator	j;		

	i = this->_clients.find(client.getSock());
	for (j = _server_sockets.begin(); j != _server_sockets.end(); j++)
	{
		if (j->poll.fd == client.getSock())
			break ;
	}
	//send the quit message to clients of the same channel
	//right now use broadcast instead
	broadcast(client, "QUIT", "Quit: ", (mess.getParam())[0]);
	//send err to client
	reply(client, "ERROR", ":client quit", NULL);
	//release resource
	close(client.getSock());
	_clients.erase(i);
	_server_sockets.erase(j);
}




/*	helper	*/

bool	Server::nick_in_use(std::string const &nick) const
{
	for (client_map::const_iterator i = _clients.begin(); i != _clients.end(); i++)
	{
		if ((i->second).getNick() && *((i->second).getNick()) == nick)
			return (true);
	}
	return (false);
}

/*
They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).

They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).

They MUST NOT start with a character listed as a channel type prefix.

They SHOULD NOT contain any dot character ('.', 0x2E).
*/
bool	Server::nick_valid(std::string const &nick)
{
	if (nick.find_first_of(" ,*?!@.") != std::string::npos)
		return (false);
	if (nick.find_first_of("$:&#") == 0)
		return (false);
	return (true);
}

void	Server::welcome_mess(Client const &client)
{
	reply(client, RPL_WELCOME, ":Welcome to our network", NULL);
	reply(client, RPL_YOURHOST, ":Your host is ft_irc", NULL);
	reply(client, RPL_CREATED, ":This version is created yesterday", NULL);
	reply(client, RPL_MYINFO, "<servername> <version> <available user modes> <available channel modes>", NULL);
	reply(client, RPL_ISUPPORT, 
		"AWAYLEN=100 CASEMAPPING=ascii CHANLIMIT=#&: CHANNELLEN=32 CHANTYPES=# HOSTLEN=64 KICKLEN=255 MAXTARGETS=100 NICKLEN=31 TOPICLEN=307 USERLEN=18", " :are supported by this server");
	/*
		The value of below isupoort parameter 
		should be decide and completed later: 

		chanmode
		elist
		maxlist
		modes
		prefix
		STATUSMSG
		TARGMAX
		EXCEPTS
	*/
}
