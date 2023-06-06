#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <iostream>
# include <new>
# include <vector>

/*
	documentation: https://modern.ircdocs.horse/#message-format
	PS: do not implement TAG

	A message can have at most 15 parameter, 512 byte
	Every message ends with \r\n, but a simple \n is acceptable
	If the \r\n is not present, it'a assumed the message is too long. The rest of the info is ignored, and it's treated as if a \r\n is at the end.
	An empty message is ignored.
*/

class Message
{
	public:
		/*	typedef	*/
		typedef std::vector<std::string>	vec_str;	

		/*	canon form	*/
		Message(void);
		Message(Message const &mess);
		~Message(void);
		Message	&operator=(Message const &mess);

		bool	parse(std::string const &cmd);

		/*	getters	*/
		std::string const	*getSource(void) const;
		std::string	const	*getCommand(void) const;
		vec_str const		&getParam(void) const;
		int					getParamNum(void) const;

	private:
		/*	private member variable	*/
		std::string		*_src;
		std::string		*_cmd;
		vec_str			_param;		

		/*	setters	*/
		void	setSource(std::string const &input, std::string::size_type &pos);
		void	setCommand(std::string const &input, std::string::size_type &pos);
		void	setParam(std::string const &input, std::string::size_type &pos);

		/*	helper function	*/
		void				clear(void);
};

std::ostream	&operator<<(std::ostream &out, Message const &mess);

#endif