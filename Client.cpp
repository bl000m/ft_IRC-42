#include "Client.hpp"

Client::Client(void)
	:_sock(-1), _pass(false), _nick(NULL), _user(NULL), _host(NULL) {}

Client::Client(Client const &client)
	:_sock(client._sock), _pass(client._pass),
	_nick(NULL), _user(NULL), _host(NULL)
{
	if (client._nick)
		_nick = new std::string(client._nick);
	if (client._user)
		_user = new std::string(client._user);
	if (client._host)
		_host = new std::string(client._host);
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
	_pass = client._pass;
	clear();
	if (client._nick)
		_nick = new std::string(client._nick);
	if (client._user)
		_user = new std::string(client._user);
	if (client._host)
		_host = new std::string(client._host);
	return (*this);
}

Client::Client(int sockfd)
	:_sock(sockfd), _pass(false), _nick(NULL), _user(NULL), _host(NULL) {}

/*
	the modification of client has certain restrictions,
	and it is up to the caller to verify that the rules are meet
	For example,
	1. Once a client sent a PASS with correct password, 
		it cannot resend the message.
	2. The client cannot change nickname to another nickname that is in use
	3. The username and hostname once set, cannot be changed.
		Attempt to resend USER command should recieve a correspondant reply
*/
void	Client::bePassed(void)
{
	_pass = true;
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
int		Client::getSock(void) const
{
	return (_sock);
}
bool	Client::isPassed(void) const
{
	return (_pass);
}

void	Client::clear(void)
{
	delete _nick;
	delete _user;
	delete _host;
	_nick = NULL;
	_user = NULL;
	_host = NULL;
}

std::ostream	&operator<<(std::ostream &out, Client const &client)
{
	out << "Client: "
		<< "  sock: " << client.getSock() << std::endl
		<< "  pass: " << client.goodPass() << std::endl
		<< "  nick: ";
	if (client.getNick())
		out << client.getNick() << std::endl;
	else
		out << "(null)" << std::endl;
	out << "  user: ";
	if (client.getUser())
		out << client.getUser() << std::endl;
	else
		out << "(null)" << std::endl;
	out << "  host: ";
	if (client.getHost())
		out << client.getHost() << std::endl;
	else
		out << "(null)" << std::endl;
	return (out);
}