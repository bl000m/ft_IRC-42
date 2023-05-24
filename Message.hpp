#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <iostream>
# include <new>

/*
	documentation: https://modern.ircdocs.horse/#message-format
	PS: do not implement TAG

	A message can have at most 15 parameter
	
*/

# define SIXTEEN 16

class Message
{
	public:
		/*	canon form	*/
		Message(void);
		Message(Message const &mess);
		~Message(void);
		Message	&operator=(Message const &mess);

		bool	parse(char buffer[512]);

		std::string const	*getSource(void) const;
		std::string	const	*getCommand(void) const;
		std::string const	**getParam(void) const;

	private:
		std::string	*_src;
		std::string	*_cmd;
		std::string	*_param[SIXTEEN];

		void	clear(void);
};

std::ostream	&operator<<(ostream &out, Message const &mess);

#endif