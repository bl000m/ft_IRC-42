#include "Server.hpp"

static void	welcome_mess(Client &client);

void	Server::pass(Client &client, Message const &mess)
{
	if (client.isRegist() || client.getNick())
	{
		client.reply(ERR_ALREADYREGISTERED, ":You may not reregister", NULL);
		// this->reply(client, ERR_ALREADYREGISTERED, ":You may not reregister", NULL);
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		client.reply(ERR_NEEDMOREPARAMS, "PASS", ":Not enough parameters");
		// this->reply(client,  ERR_NEEDMOREPARAMS, "PASS", ":Not enough parameters");
		return ;
	}
	if (mess.getParam().front() != this->_password)
	{
		client.reply(ERR_PASSWDMISMATCH, ":wrong password", NULL);
		// this->reply(client,  ERR_PASSWDMISMATCH, ":wrong password", NULL);
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
		client.reply(ERR_UNKNOWNCOMMAND, "NICK", ":Unknown command");
		// this->reply(client, ERR_UNKNOWNCOMMAND, "NICK", ":Unknown command");
		return ;
	}
	if (mess.getParamNum() < 1)
	{
		client.reply(ERR_NONICKNAMEGIVEN, ":No nickname given", NULL);
		// this->reply(client, ERR_NONICKNAMEGIVEN, ":No nickname given", NULL);
		return ;
	}
	new_nick = mess.getParam().front();
	if (nick_valid(new_nick) == false)
	{
		client.reply(ERR_ERRONEUSNICKNAME, new_nick.c_str(), ":Erroneus nickname");
		// this->reply(client, ERR_ERRONEUSNICKNAME, new_nick.c_str(), ":Erroneus nickname");
		return ;
	}
	if (client.getNick() && *client.getNick() == new_nick)
		return ;
	if (nick_in_use(new_nick))
	{
		client.reply(ERR_NICKNAMEINUSE, ":Nickname is already in use", NULL);
		// this->reply(client, ERR_NICKNAMEINUSE, ":Nickname is already in use", NULL);
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
		client.reply(ERR_ALREADYREGISTERED, ":You may not reregister", NULL);
		// this->reply(client, ERR_ALREADYREGISTERED, ":You may not reregister", NULL);
		return ;
	}
	if (client.getNick() == NULL)
	{
		client.reply(ERR_UNKNOWNCOMMAND, "USER", ":Unknown command");
		// this->reply(client, ERR_UNKNOWNCOMMAND, "USER", ":Unknown command");
		return ;
	}
	if (param.size() < 4 || param[0].size() < 1)
	{
		client.reply(ERR_NEEDMOREPARAMS, "USER", ":Not enough parameters");
		// this->reply(client, ERR_NEEDMOREPARAMS, "USER", ":Not enough parameters");
		return ;
	}
	//trunc user name if required
	client.setUser(param[0]);
	client.setHost(param[3]);
	client.beRegist();
	welcome_mess(client);
}

void	Server::quit(Client &client, Message const &mess)
{
	std::string		note;
	channelListIt		i;


	note = ":" + client.getFullName() + " QUIT Quit: ";
	if (mess.getParamNum() > 0)
	{
		note += mess.getParam()[0].c_str();	
	}
	for (i = _channels.begin(); i != _channels.end(); i++)
	{
		if (i->second.isUserInChannel(*client.getNick()))
		{
			i->second.broadcast(note, client);
			i->second.removeChannelUser(*client.getNick());
		}
	}
	note = ":localhost ERROR :client quit";
	client.reply(note.c_str());
	// send(client.getSock(), note.c_str(), note.size(), 0);
	rmClient(client);
}

void	Server::ping(Client &client, Message const &mess)
{
	std::string	note = ":localhost PONG localhost ";
	
	if (mess.getParamNum() < 1)
	{
		client.reply(ERR_NEEDMOREPARAMS, "PING", ":Not enough parammeter");
		// reply(client, ERR_NEEDMOREPARAMS, "PING", ":Not enough parammeter");
		return ;
	}
	note = note + mess.getParam()[0];
	client.reply(note.c_str());
	// send(client.getSock(), note.c_str(), note.size(), 0);
}

void	Server::pong(Client &client, Message const &mess)
{
	(void) client;
	(void) mess;
}

void	Server::oper(Client &client, Message const &mess)
{
	if (client.isServerOp())
		return ;
	if (mess.getParamNum() < 2)
	{
		client.reply(ERR_NEEDMOREPARAMS, "OPER", ":Not enough parammeter");
		// reply(client, ERR_NEEDMOREPARAMS, "OPER", ":Not enough parammeter");
		return ;
	}
	if (mess.getParam()[1] != _password)
	{
		client.reply(ERR_PASSWDMISMATCH, ":password incorrect", NULL);
		// reply(client, ERR_PASSWDMISMATCH, ":password incorrect", NULL);
		return ;
	}
	client.setServerOp(true);
	client.reply(RPL_YOUREOPER, ":You are now an IRC operator", NULL);
	client.reply("MODE", client.getMode().c_str(), NULL);
	// reply(client, RPL_YOUREOPER, ":You are now an IRC operator", NULL);
	// reply(client, "MODE", client.getMode().c_str(), NULL);
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

/*
	Not all isupport message are implemented
*/
static void	welcome_mess(Client &client)
{
	client.reply(RPL_WELCOME, ":Welcome to our network", NULL);
	client.reply(RPL_YOURHOST, ":Your host is ft_irc", NULL);
	client.reply(RPL_CREATED, ":This version is created yesterday", NULL);
	client.reply(RPL_MYINFO, "<servername> <version> <available user modes> <available channel modes>", NULL);
	client.reply(RPL_ISUPPORT, "CASEMAPPING=ascii CHANLIMIT=#&: CHANTYPES=#", ":are supported by this server");
	client.reply(RPL_UMODEIS, client.getMode().c_str(), NULL);
	client.reply(ERR_NOMOTD, "no MOTD", NULL);
}
