#include "Client.hpp"

/*	canon form	*/
Client::Client(void)
	:_regist(false), _invisible(false),
	_server_op(false), _wallop(true),
	_sock(-1), _pass(false),
	_nick(NULL), _user(NULL), _host(NULL),
	_sock_len(-1)
{
	std::memset(&_sock_addr, 0, sizeof(sockaddr_in));
	memset(_buff, 0, MAX_BUFFER);
}

Client::Client(Client const &client)
	:_regist(client._regist), _invisible(client._invisible),
	_server_op(client._server_op), _wallop(client._wallop),
	_sock(client._sock), _pass(client._pass),
	_nick(NULL), _user(NULL), _host(NULL),
	_sock_len(client._sock_len),
	_sock_addr(client._sock_addr)
{
	if (client._nick)
		_nick = new std::string(*client._nick);
	if (client._user)
		_user = new std::string(*client._user);
	if (client._host)
		_host = new std::string(*client._host);
	memset(_buff, 0, MAX_BUFFER);
}

Client::~Client(void)
{
	clear();
}

Client	&Client::operator=(Client const &client)
{
	if (this == &client)
		return (*this);
	_regist = client._regist;
	_invisible = client._invisible;
	_server_op = client._server_op;
	_wallop = client._wallop;

	_sock = client._sock;
	_pass = client._pass;
	_sock_len = client._sock_len;
	_sock_addr = client._sock_addr;
	clear();
	if (client._nick)
		_nick = new std::string(*client._nick);
	if (client._user)
		_user = new std::string(*client._user);
	if (client._host)
		_host = new std::string(*client._host);
	return (*this);
}

/*	ctor with arguments	*/
Client::Client(int sockfd, socklen_t socklen, sockaddr_in sockaddr)
	:_regist(false), _invisible(false), _server_op(false), _wallop(true),
	_sock(sockfd), _pass(false),
	_nick(NULL), _user(NULL), _host(NULL),
	_sock_len(socklen), _sock_addr(sockaddr) {}

/*
	the modification of client has certain restrictions,
	and it is up to the caller to verify that the rules are meet
	For example,
	1. Server should verify if there's nickname collsion
	2. Once register, PASS and USER should not be accepted, and should generate an error message.
*/

/*	setters	*/
void	Client::setPass(bool good)
{
	_pass = good;
}
void	Client::beRegist(void)
{
	_regist = true;
}

void	Client::setNick(std::string const &nick)
{
	delete _nick;
	_nick = new std::string(nick);
}
void	Client::setUser(std::string const &user)
{
	delete _user;
	_user = new std::string(user);
}
void	Client::setHost(std::string const &host)
{
	delete _host;
	_host = new std::string(host);
}
void	Client::setServerOp(bool yes)
{
	_server_op = yes;
}
void	Client::setInvisible(bool yes)
{
	_invisible = yes;
}
void	Client::setWallop(bool yes)
{
	_wallop = yes;
}
bool	Client::setMode(std::string mode)
{
	std::string::size_type	i;
	bool	unknown = false;
	bool	op = true;

	if (mode.size() < 1 || (mode[0] != '+' && mode[0] != '-'))
		return (false);
	for (i = 0; i < mode.size(); i++)
	{
		switch(static_cast<int>(mode[i]))
		{
			case '+':
				op = true;
				break ;
			case '-':
				op = false;
				break ;
			case 'i':
				setInvisible(op);
				break ;
			case 'r':
				break ;
			case 'w':
				setWallop(op);
				break ;
			case 'o':
				if (op == false)
					setServerOp(op);
				break ;
			default:
				unknown = true;
		}
	}
	return (unknown);
}


/*	getters	*/
int		Client::getSock(void) const
{
	return (_sock);
}
bool	Client::getPass(void) const
{
	return (_pass);
}
bool	Client::isRegist(void) const
{
	return (_regist);
}
std::string const	*Client::getNick(void) const
{
	return (_nick);
}
std::string const	*Client::getUser(void) const
{
	return (_user);
}
std::string const	*Client::getHost(void) const
{
	return (_host);
}
std::string		Client::getFullName(void) const
{
	std::string	nick = "unknown";
	std::string	user = "unknown";
	std::string	host = "unknown";

	if (_nick)
		nick = *_nick;
	if (_user)
		user = *_user;
	if (_host)
		host = *_host;
	return (nick + "!" + user + "@" + host);
}
bool	Client::isServerOp(void) const
{
	return (_server_op);
}
bool	Client::isInvisible(void) const
{
	return (_invisible);
}
bool	Client::getWallop(void) const
{
	return (_wallop);
}
std::string		Client::getMode(void) const
{
	std::string	temp = "+";

	if (_invisible)
		temp += "i";
	if (_server_op)
		temp += "o";
	if (_regist)
		temp += "r";
	if (_wallop)
		temp += "w";
	return (temp);
}

std::string	&Client::getBuff(void)
{
	return (_strbuff);
}

void	Client::clearBuff(void)
{
	_strbuff.clear();
}

void	Client::catBuff(char *buff, int size)
{
	int	nbuff_size;

	nbuff_size = size + strlen(_buff);
	(void)nbuff_size;
	_strbuff += buff;
}

/*	private function	*/
void	Client::clear(void)
{
	delete _nick;
	delete _user;
	delete _host;
	_nick = NULL;
	_user = NULL;
	_host = NULL;
}

/*	opeartor<< overload	*/
std::ostream	&operator<<(std::ostream &out, Client const &client)
{
	out << "Client:  sock(" << client.getSock() << ")"
		<< client.getMode() << std::endl
		<< "  nick: ";
	if (client.getNick())
		out << *client.getNick() << std::endl;
	else
		out << "(null)" << std::endl;
	out << "  user: ";
	if (client.getUser())
		out << *client.getUser() << std::endl;
	else
		out << "(null)" << std::endl;
	out << "  host: ";
	if (client.getHost())
		out << *client.getHost() << std::endl;
	else
		out << "(null)" << std::endl;
	return (out);
}

/* channel related */

void Client::addChannel(Channel *newChannel){
	_channelsMember[newChannel->getName()] = newChannel;

}

void Client::removeChannel(const std::string &channelName){
	_channelsMember.erase(channelName);
}
