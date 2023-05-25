#include "Client.hpp"

struct Client::s_client
{
	std::string	*nick;
	std::string	*realname;	//address of the host
	std::string	*username;	//name of the client on that host
	/*	
		the server to which the client is connected to 
		is not recorded
	*/
};

Client::Client(void)
{
	_info.nick = NULL;
	_info.realname = NULL;
	_info.username = NULL;
}

Client::Client(Client const &client)
{
	_info.nick = NULL;
	_info.realname = NULL;
	_info.username = NULL;
	if (client._info.nick)
		_info.nick = new std::string(client._info.nick);
	if (client._info.realname)
		_info.realname = new std::string(client._info.realname);
	if (client._info.username)
		_info.username = new std::string(client._info.username);
}

Client::~Client(void)
{
	this->clear();
}

Client	&Client::operator=(Client const &client)
{
	if (this == &client)
		return (*this);
	this->clear();
	if (client._info.nick)
		_info.nick = new std::string(client._info.nick);
	if (client._info.realname)
		_info.realname = new std::string(client._info.realname);
	if (client._info.username)
		_info.username = new std::string(client._info.username);
	return (*this);
}

void	Client::clear(void)
{
	delete _info.nick;
	delete _info.realname;
	delete _info.username;
	_info.nick = NULL;
	_info.realname = NULL;
	_info.username = NULL;
}

std::string const	&Client::getNick(void) const
{
	return (_info.nick);
}

std::string const	&Client::getReal(void) const
{
	return (_info.realname);
}
std::string const	&Client::getUser(void) const
{
	return (_info.username);
}
Client::client_map const	&Client::getWhoswho(void) const
{
	return (_whos_who);
}

/*	authenticate and add name to list	*/
bool	Client::regist(int sock)
{
	std::string	nick;
	std::string	real;
	std::string	user;

	this->clear();
	//authenticate, record, verify all is good
	_info.nick = nick;
	_info.realname = realname;
	_info.username = username;
	_whos_who.insert(std::pair<std::string, t_client>(nick, _info));
}

/*	remove name from list and delete resourses	*/
void	Client::quit(void)
{
	client_map::iterator	it;

	it = _whos_who.find(_info.nick);
	if (it == _whos_who.end())
	{
		std::cerr << "Client: " << _info.nick
		<< " not found" << std::endl;
		return ;
	}
	_whos_who.erase(it);
	this->clear();
}

std::ostream	&operator<<(std::ostream &out, Client const &client)
{
	if (client.getNick() && client.getUser() && client.getReal())
	{
		out << "Client: " << client.getNick()
			<< " !" << client.getUser() << "@" << client.getReal()
			<< std::endl;
		return (out);
	}
	std::cout << "Client: (empty)" << std::endl;
	return (out);
}