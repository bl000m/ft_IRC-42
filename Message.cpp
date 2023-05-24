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
		_src = new mess._src;
	if (mess._cmd)
		_cmd = new mess._cmd;
	for (int i = 0; i < SIXTEEN; i++)
	{
		if (mess._param[i])
			_param[i] = new mess._param[i];
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
		_src = new mess._src;
	if (mess._cmd)
		_cmd = new mess._cmd;
	for (int i = 0; i < SIXTEEN; i++)
	{
		if (mess._param[i])
			_param[i] = new mess._param[i];
	}
	return (*this);
}

bool	Message::parse(char buffer[512])
{
	//check \r\n present
	//get source
	//get cmd
	//get param
}

std::string const	*Message::getSource(void) const
{
	return (_src);
}

std::string	const	*Message::getCommand(void) const
{
	return (_cmd);
}

std::string const	**Message::getParam(void) const
{
	return (_param);
}

