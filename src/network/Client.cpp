#include "Client.hpp"

/*	canon form	*/
Client::Client(void)
	:_regist(false), _invisible(false),
	_server_op(false), _wallop(true),
	_sock(-1), _pass(false),
	_nick(NULL), _user(NULL), _host(NULL),
	_sock_len(-1), _envelope(""), _quit(false),
	_away("")
{
	std::memset(&_sock_addr, 0, sizeof(sockaddr_in));
}

Client::Client(Client const &client)
	:_regist(client._regist), _invisible(client._invisible),
	_server_op(client._server_op), _wallop(client._wallop),
	_sock(client._sock), _pass(client._pass),
	_nick(NULL), _user(NULL), _host(NULL),
	_sock_len(client._sock_len),
	_sock_addr(client._sock_addr), _envelope(""), _quit(client._quit), _away(client._away)
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
	_regist = client._regist;
	_invisible = client._invisible;
	_server_op = client._server_op;
	_wallop = client._wallop;

	_sock = client._sock;
	_pass = client._pass;
	_sock_len = client._sock_len;
	_sock_addr = client._sock_addr;
	_envelope = client._envelope;
	_quit = client._quit;
	_away = client._away;
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
	_sock_len(socklen), _sock_addr(sockaddr),
	_envelope(""), _quit(false),
	_away("") {}

/*
	the modification of client has certain restrictions,
	and it is up to the caller to verify that the rules are meet
	For example,
	1. Server should verify if there's nickname collsion
	2. Once register, PASS and USER should not be accepted, and should generate an error message.
*/

/*	reply-to-client related function	*/
void	Client::reply(char const *numeric, char const *p1, char const *p2)
{
	_envelope += ":localhost ";
	if (numeric)
		_envelope += numeric;
	if (_nick)
		_envelope = _envelope + " " + *_nick;
	else
		_envelope = _envelope + " unknown";
	if (p1)
		_envelope = _envelope + " " + p1;
	if (p2)
		_envelope = _envelope + " " + p2;
	if (_envelope[_envelope.size() - 1] != '\n')
		_envelope += "\r\n";
}
void	Client::reply(char *src, char *cmd, char *p1, char *p2)
{
	if (src)
		_envelope = _envelope + ":" + src;
	if (cmd)
		_envelope = _envelope + " " + cmd;
	if (p1)
		_envelope = _envelope + " " + p1;
	if (p2)
		_envelope = _envelope + " " + p2;
	if (_envelope[_envelope.size() - 1] != '\n')
		_envelope += "\r\n";
}
void	Client::reply(char *src, char *cmd, char *p1, char *p2, char *p3)
{
	if (src)
		_envelope = _envelope + ":" + src;
	if (cmd)
		_envelope = _envelope + " " + cmd;
	if (p1)
		_envelope = _envelope + " " + p1;
	if (p2)
		_envelope = _envelope + " " + p2;
	if (p3)
		_envelope = _envelope + " " + p3;
	if (_envelope[_envelope.size() - 1] != '\n')
		_envelope += "\r\n";
}
void	Client::reply(char const *note)
{
	_envelope += note;
	if (_envelope[_envelope.size() - 1] != '\n')
		_envelope += "\r\n";
}
void	Client::beSent(void)
{
	if (_envelope.empty())
		return ;
	send(_sock, _envelope.c_str(), _envelope.size(), 0);
	_envelope.clear();
}

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
void	Client::beQuit(void)
{
	_quit = true;
}
void	Client::setAway(std::string mess)
{
	_away = mess;
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

bool	Client::isQuit(void) const
{
	return (_quit);
}

std::string	Client::getAway(void) const
{
	return (_away);
}

std::string	&Client::getBuff(void)
{
	return (_readbuf);
}

void	Client::clearBuff(void)
{
	std::string::size_type	i;

	/*leave the last line if it's not finished*/
	if (_readbuf[_readbuf.size() - 1] == '\n')
	{
		_readbuf.clear();
		return ;
	}
	if (_readbuf.find("\n") == std::string::npos)
		return ;
	i = _readbuf.find_last_of("\n");
	_readbuf = _readbuf.substr(i + 1);
}

void	Client::catBuff(char *buff, int size)
{
	bool	end;

	end = false;
	for (int i = 0; i < size + 1; i++)
	{
		if (buff[i] == '\0')
			end = true;
	}
	if (!end)
		std::cout << "Client: catbuff: input has no \\0" << std::endl;
	else
		_readbuf += buff;
	// int	nbuff_size;

	// nbuff_size = size + strlen(_buff);
	// (void)nbuff_size;
	// _strbuff += buff;
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

void Client::addChannel(Channel *newChannel, std::string channelName){
	_channelsMember[channelName] = newChannel;
	std::cout << "channel: " << channelName << "has been added to " << *(this->getNick()) << " channels list" << std::endl;
	std::cout << *(this->getNick()) << " channels list updated:" << std::endl;
	for (channelMapIt it = _channelsMember.begin(); it != _channelsMember.end(); it++){
			std::cout << it->first << "\n" << std::endl;
		}
}

void Client::removeChannel(const std::string &channelName){
	_channelsMember.erase(channelName);
}

Client::channelMap Client::getChannels() const{
	return _channelsMember;
}
