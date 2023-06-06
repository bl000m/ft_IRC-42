#include "Message.hpp"

Message::Message(void)
	:_src(NULL), _cmd(NULL), _param() {}

Message::Message(Message const &mess)
	:_src(NULL), _cmd(NULL), _param()
{
	if (mess._src)
		_src = new std::string(*mess._src);
	if (mess._cmd)
		_cmd = new std::string(*mess._cmd);
	_param = mess._param;
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
	_param.clear();
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
	_param = mess._param;
	return (*this);
}

/*
	return true when parsing succeed
	return false when parsing fails (ex. empty message, in this case, it must be silently ignored)
*/
bool	Message::parse(std::string cmd)
{
	std::string::size_type	pos;
	
	this->clear();
	if (cmd.size() == 0
		|| (cmd.size() == 1 && cmd[0] == '\r'))
		return (false);
	if (cmd[cmd.size() - 1] == '\r')
		cmd = cmd.substr(0, cmd.size() - 1);
	pos = 0;
	this->setSource(cmd, pos);
	this->setCommand(cmd, pos);
	this->setParam(cmd, pos);
	if (_cmd == NULL)
		return (false);
	return (true);
}

void	Message::setSource(std::string const &input, std::string::size_type &pos)
{	
	pos = 0;
	if (*(input.begin()) != ':')
	{
		pos = input.find_first_not_of(" ", pos);
		return ;
	}
	pos = input.find(" ", 1);
	if (pos == std::string::npos)
	{
		return ;
	}
	if (pos == 1)
	{
		pos = input.find_first_not_of(" ", pos);
		return ;
	}
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
	if (pos == end)
		return ;
	_cmd = new std::string(input.substr(pos, end - pos));
	pos = end;
}

void	Message::setParam(std::string const &input, std::string::size_type &pos)
{
	std::string::size_type	end;

	while (pos != std::string::npos)
	{
		pos = input.find_first_not_of(" ", pos);
		end = input.find(" ", pos);
		if (pos == end)
			return ;
		if (input[pos] == ':')
		{
			_param.push_back(input.substr(pos + 1, std::string::npos));
			return ;
		}
		_param.push_back(input.substr(pos, end - pos));
		pos = end;
	}
}

std::string const	*Message::getSource(void) const
{
	return (_src);
}

std::string	const	*Message::getCommand(void) const
{
	return (_cmd);
}

Message::vec_str const	&Message::getParam(void) const
{
	return (_param);
}

int		Message::getParamNum(void) const
{
	return (_param.size());
}


std::ostream	&operator<<(std::ostream &out, Message const &mess)
{
	std::string const				*src = mess.getSource();
	std::string	const				*cmd = mess.getCommand();
	std::vector<std::string> const	param = mess.getParam();
	
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
	for (Message::vec_str::const_iterator i = param.begin(); i != param.end(); i++)
	{
		out << *i << ", ";
	}
	out << "(end)" << std::endl;
	out << "Parameter #: " << mess.getParamNum() << std::endl;
	return (out);
}
