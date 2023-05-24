#include "Message.hpp"

Message::Message(void)
	:_src(NULL), _cmd(NULL)
{
	for (int i = 0; i < SIXTEEN; i++)
		_param[i] = NULL;
}

Message::Message(Message const &mess)
{
	if (mess._src)
		_src = new std::string(*mess._src);
	if (mess._cmd)
		_cmd = new std::string(*mess._cmd);
	for (int i = 0; i < SIXTEEN; i++)
	{
		if (mess._param[i])
			_param[i] = new std::string(*mess._param[i]);
	}
}

Message::~Message(void)
{
	this->clear();
}

void	Message::clear(void)
{
	delete _src;
	_src = NULL;
	delete _cmd;
	_cmd = NULL;
	for (int i = 0; i < SIXTEEN; i++)
	{
		delete _param[i];
		_param[i] = NULL;
	}
	return ;
}

Message	&Message::operator=(Message const &mess)
{
	if (this == &mess)
		return (*this);
	this->clear();
	if (mess._src)
		_src = new std::string(*mess._src);
	if (mess._cmd)
		_cmd = new std::string(*mess._cmd);
	for (int i = 0; i < SIXTEEN; i++)
	{
		if (mess._param[i])
			_param[i] = new std::string(*mess._param[i]);
	}
	return (*this);
}

/*
	return true when parsing succeed
	return false when parsing fails (ex. empty message, in this case, it must be silently ignored)
*/
bool	Message::parse(char buffer[512])
{
	std::string				input;
	std::string::size_type	pos;
	
	this->clear();
	if (buffer_empty(buffer))
		return (false);
	input = read_buffer(buffer);
	std::cout << input << std::endl;
	pos = 0;
	this->setSource(input, pos);
	this->setCommand(input, pos);
	this->setParam(input, pos);
	if (_cmd == NULL)
		return (false);
	return (true);
}

void	Message::setSource(std::string const &input, std::string::size_type &pos)
{	
	pos = 0;
	if (*(input.begin()) != ':')
		return ;
	pos = input.find(" ", 0);
	_src = new std::string(input.substr(1, pos - 1));
	if (pos == std::string::npos)
		return ;
	pos = input.find_first_not_of(" ", pos);
}

void	Message::setCommand(std::string const &input, std::string::size_type &pos)
{
	std::string::size_type	end;

	if (pos == std::string::npos)
		return ;
	end = input.find(" ", pos);
	_cmd = new std::string(input.substr(pos, end - pos));
	// std::cout << *_cmd << std::endl;
	pos = end;
}

void	Message::setParam(std::string const &input, std::string::size_type &pos)
{
	std::string::size_type	end;
	int						i;

	i = 0;
	while (pos != std::string::npos && i < 15)
	{
		pos = input.find_first_not_of(" ", pos);
		end = input.find(" ", pos);
		if (input[pos] == ':')
		{
			_param[i] = new std::string(input.substr(pos + 1, std::string::npos));
			return ;
		}
		_param[i] = new std::string(input.substr(pos, end - pos));
		i++;
		pos = end;
	}
}

/*
	look for \r\n in buffer, and replace it with \0
	if \r\n is not present, look for \n
	if even \n is not present, 
	it acts as if \r\n are at the end of buffer
*/
std::string	Message::read_buffer(char buff[512])
{
	std::string	temp;
	int			i;
	
	i = 0;
	while (i < 512)
	{
		if (buff[i] == '\n')
			break ;
		i++;
	}
	if (i == 512)
		buff[510] = '\0';
	else if (buff[i - 1] == '\r')
		buff[i - 1] = '\0';
	else
		buff[i] = '\0';
	temp = buff;
	return (temp);
}

/*
	message is ended with \r\n,
	if it's ended with a single \n, although incorrect,
	must be handled as well
*/
bool	Message::buffer_empty(char buf[512])
{
	if (buf[0] == '\r' && buf[1] == '\n')
		return (true);
	if (buf[0] == '\n')
		return (true);
	return (false);
}


std::string const	*Message::getSource(void) const
{
	return (_src);
}

std::string	const	*Message::getCommand(void) const
{
	return (_cmd);
}

const std::string* const*	Message::getParam(void) const
{
	return (_param);
}

std::ostream	&operator<<(std::ostream &out, Message const &mess)
{
	std::string const			*src = mess.getSource();
	std::string	const			*cmd = mess.getCommand();
	const std::string* const	*param = mess.getParam();
	
	out << "message:" << std::endl << "  source: ";
	if (src)
		out << *src;
	else
		out << "(null)";
	out << std::endl << "  command: ";
	if (cmd)
		out << *cmd;
	else
		out << "(null)";
	out << std::endl << "  param: ";
	for (int i = 0; i < SIXTEEN; i++)
	{
		if (param[i])
			out << *(param[i]) << ", ";
		else
		{
			out << "(null)" << std::endl;
			return (out);
		}
	}
	return (out);
}
