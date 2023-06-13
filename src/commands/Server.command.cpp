#include "Server.hpp"

void	Server::execMessage(Client &client, Message const &mess)
{
	fn_ptr				cmd_ptr;
	std::string const	*cmd;

	cmd = mess.getCommand();
	if (!cmd)
		return ;
	cmd_ptr = getCmd(*cmd);
	if (!cmd_ptr)
	{
		this->reply(client, ERR_UNKNOWNCOMMAND, cmd->c_str(), ":Unknown command");
		return ;
	}
	if (!client.isRegist()
			&& !(*cmd == "PASS" || *cmd == "NICK" || *cmd == "USER"))
	{
		this->reply(client, ERR_UNKNOWNCOMMAND, cmd->c_str(), ":Unknown k command");
		return ;
	}
	(this->*cmd_ptr)(client, mess);
}

Server::fn_ptr	Server::getCmd(std::string const &cmd)
{
	fn_map::const_iterator	i;

	i = _command.find(cmd);
	if (i == _command.end())
		return (NULL);
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
	client_map const	&clients = this->_clients;

	note = ":" + src;
	if (cmd)
		note = note + " " + cmd;
	if (p1)
		note = note + " " + p1;
	if (p2)
		note = note + " " + p2;
	note += "\r\n";
	for (client_map::const_iterator i = clients.begin(); i != clients.end(); i++)
	{
		if (i->second.getFullName() == src)
			continue ;
		send(i->second.getSock(), note.c_str(), note.size(), 0);
	}
	return ;
}

Server::fn_map	Server::cmd_init(void)
{
	std::map<std::string, Server::fn_ptr>	temp;

	temp["PASS"] = &Server::pass;
	temp["NICK"] = &Server::nick;
	temp["USER"] = &Server::user;
	temp["QUIT"] = &Server::quit;
	temp["PING"] = &Server::ping;
	temp["PONG"] = &Server::pong;
	temp["OPER"] = &Server::oper;
	temp["PRIVMSG"] = &Server::privmsg;
	temp["NOTICE"] = &Server::notice;
	temp["MODE"] = &Server::mode;
	temp["MOTD"] = &Server::motd;
	temp["WALLOPS"] = &Server::wallops;
	temp["KILL"] = &Server::kill;
	temp["JOIN"] = &Server::join;
	temp["INVITE"] = &Server::invite;
	temp["KICK"] = &Server::kick;
	temp["TOPIC"] = &Server::topic;

	return (temp);
}

Server::fn_map const	Server::_command = cmd_init();

void	Server::force_quit(int sock, bool err)
{
	Client				*client;
	std::string 		str_err;
	std::string			note;
	channelListIt		i;

	if (_clients.find(sock) == _clients.end())
		return ;
	client = &(_clients.find(sock)->second);
	if (err)
	{
		reply(*client, ERR_INPUTTOOLONG, ":Input too long", NULL);
	}
	if (!client->isRegist())
	{
		rmClient(*client);
		return ;
	}
	note = ":" + client->getFullName() + " QUIT :force quit\r\n";
	for (i = _channels.begin(); i != _channels.end(); i++)
	{
		if (i->second.isUserInChannel(*client->getNick()))
		{
			i->second.broadcast(note, *client);
			i->second.removeChannelUser(*client->getNick());
		}
	}
	rmClient(*client);
}

Client	*Server::getClient(std::string const &nick)
{
	Client					*temp;
	client_map::iterator	i;

	temp = NULL;
	for (i = _clients.begin(); i != _clients.end(); i++)
	{
		if (*(i->second.getNick()) == nick)
			temp = &(i->second);
	}
	return (temp);
}

void	Server::rmClient(Client &client)
{
	client_map::iterator			i;
	std::vector<pollfd>::iterator	j;

	i = _clients.find(client.getSock());
	for (j = _server_sockets.begin(); j != _server_sockets.end(); j++)
	{
		if (j->fd == client.getSock())
			break ;
	}
	close(client.getSock());
	//erase clients from client list and poll list
	if (i != _clients.end())
		_clients.erase(i);
	if (j != _server_sockets.end())
		_server_sockets.erase(j);
}

Channel		*Server::getChan(std::string const &chan)
{
	channelListIt	i;
	
	i = _channels.find(chan);
	if (i == _channels.end())
		return (NULL);
	return (&(i->second));
}

void	Server::rmChan(std::string const &chan)
{
	channelListIt	i;
	
	i = _channels.find(chan);
	if (i == _channels.end())
		return ;
	_channels.erase(i);
}
