#include "Client.hpp"

/*	canon form	*/
Client::Client(void)
	:_sock(-1), _ip(0), _pass(false), _regist(false),
	_nick(NULL), _user(NULL), _host(NULL) {}

Client::Client(Client const &client)
	:_sock(client._sock), _ip(client._ip),
	_pass(client._pass), _regist(client._regist)
	_nick(NULL), _user(NULL), _host(NULL)
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
	_ip = client._ip;
	_pass = client._pass;
	_regist = client._regist;
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
Client::Client(int sockfd, unsigned int ip)
	:_sock(sockfd), _ip(ip), _pass(false), _regist(false),
	_nick(NULL), _user(NULL), _host(NULL) {}

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
void	Client::beRegist(void) const
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

/*	getters	*/
int		Client::getSock(void) const
{
	return (_sock);
}
unsigned int	Client::getIp(void) const
{
	return (_ip);
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
	out << "Client: "
		<< "  sock: " << client.getSock() << std::endl
		<< "  ip: " << client.getIp() << std::endl
		<< "  pass: " << client.isPassed() << std::endl
		<< "  regist: " << client.isRegist() << std::endl
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