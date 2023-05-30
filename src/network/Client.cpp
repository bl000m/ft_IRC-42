#include "Client.hpp"

/*	canon form	*/
Client::Client(void)
	:_sock(-1), _pass(false), _regist(false),
	_nick(NULL), _user(NULL), _host(NULL),
	_sock_len(-1), _server_op(false)
{
	std::memset(&_sock_addr, 0, sizeof(sockaddr_in));
}

Client::Client(Client const &client)
	:_sock(client._sock), _pass(client._pass),
	_regist(client._regist),
	_nick(NULL), _user(NULL), _host(NULL),
	_sock_len(client._sock_len),
	_sock_addr(client._sock_addr), _server_op(false)
{
	if (client._nick)
		_nick = new std::string(*client._nick);
	if (client._user)
		_user = new std::string(*client._user);
	if (client._host)
		_host = new std::string(*client._host);
}

Client::~Client(void)
{
	clear();
}

Client	&Client::operator=(Client const &client)
{
	if (this == &client)
		return (*this);
	_sock = client._sock;
	_sock_len = client._sock_len;
	_sock_addr = client._sock_addr;
	_pass = client._pass;
	_regist = client._regist;
	_server_op = client._server_op;
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
	:_sock(sockfd), _pass(false), _regist(false),
	_nick(NULL), _user(NULL), _host(NULL),
	_sock_len(socklen), _sock_addr(sockaddr), _server_op(false) {}

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
		<< "  pass(" << client.getPass() << ")"
		<< "  regist(" << client.isRegist() << ")" << std::endl
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