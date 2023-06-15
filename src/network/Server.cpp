//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER

#include "Server.hpp"
#include "Message.hpp"

bool is_running = true;

Server::Server()
{}

Server::~Server() {
	std::for_each(_server_sockets.begin(), _server_sockets.end(), closeSocket);
}

void    Server::initServer(const std::string &port, const std::string &password)
{
	std::stringstream	ss;
	
	_password = password;
	ss << port;
	ss >> _iport;
	if (ss.fail() || !(_iport > MIN_PORT && _iport < MAX_PORT))
		throw std::runtime_error("Invalid port");
	if (initServerPoll() == false)
		throw std::runtime_error("Init Server Error: " + std::string(strerror(errno)));
	std::cout << "Server Init done" << std::endl;
}

void	Server::run(void)
{
	int			poll_ret;
	pollfd		*current_poll;
	char		buffer[MAX_BUFFER + 1];

	std::cout << "Running Server" << std::endl;
	while (is_running)
	{
		_server_sockets[0].revents = 0;
		poll_ret = poll(_server_sockets.data(), _server_sockets.size(), POLL_DELAY);
		if (!is_running)
			break;
		if (poll_ret < 0)
		{
			perror("poll()");
			return ;
		}
		if (IS_POLLIN(_server_sockets[0].revents))
			newClientPoll();
		for (size_t i = 1; i < _server_sockets.size(); i++)
		{
			current_poll = &_server_sockets[i];
			if (IS_POLLOUT(current_poll->revents))
			{
				client_pollout(current_poll->fd);
			}
			if (IS_POLLIN(current_poll->revents))
			{
				std::cout << "POLLIN: ";
				client_pollin(buffer, current_poll->fd);
			}
			if (IS_POLLHUP(current_poll->revents))
			{
				std::cout << "POLLHUP" << std::endl;
				force_quit(current_poll->fd, false);
			}
			if (IS_POLLERR(current_poll->revents))
			{
				std::cout << "POLLERR" << std::endl;
				force_quit(current_poll->fd, false);
			}
		}
	}
}

void	Server::client_pollout(int sock)
{
	Client	*client;

	client = &(_clients.find(sock)->second);
	client->beSent();
	if (client->isQuit())
		rmClient(*client);
}

void	Server::client_pollin(char *buf, int sock)
{
	int							ret;
	Client						*client;
	Message						mess;
	std::vector<std::string>	cmds;

	client = &(_clients.find(sock)->second);
	for (int i = 0; ; i++)
	{
		memset(buf, 0, MAX_BUFFER + 1);
		ret = recv(sock, buf, MAX_BUFFER, 0);
		if (ret <= CLOSE_SOCKET && i == 0)
		{
			std::cout << "force_quit" << std::endl;
			force_quit(sock, false);
			return ;
		}
		if (ret <= CLOSE_SOCKET)
			break ;
		std::cout << buf;
		client->catBuff(buf, ret);
	}
	cmds = splitCommands(client->getBuff());
	client->clearBuff();
	for (size_t j = 0; j < cmds.size(); j++)
	{
		if (mess.parse(cmds[j]))
			execMessage(_clients.find(sock)->second, mess);
	}
}





std::vector<std::string>	Server::splitCommands(std::string &buffer)
{
	std::stringstream			ss;
	std::string					parsed;
	std::vector<std::string>	cmds;

	ss << buffer;
	while (true)
	{
		getline(ss, parsed, '\n');
		if (ss.eof())
			return (cmds);
		cmds.push_back(parsed);
	}
	// return (cmds);
}

bool	Server::initServerPoll(void)
{
	int			new_socket;
	pollfd		new_poll;
	int			opt_arg = 1;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket < 0)
		return (false);
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_iport);
	_addr.sin_addr.s_addr = INADDR_ANY;
	if (setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &opt_arg, sizeof(int)) < 0)
		return (close(new_socket) ,false);
	if (setsockopt(new_socket, SOL_SOCKET, SO_REUSEPORT, &opt_arg, sizeof(int)) < 0)
		return (close(new_socket) ,false);
	if (bind(new_socket, reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr)) < 0)
		return (close(new_socket) ,false);
	if (listen(new_socket, MAX_QUEUE_CONNECTION) < 0)
		return (close(new_socket) ,false);
	new_poll.fd = new_socket;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_server_sockets.push_back(new_poll);
	return (true);
}

bool	Server::newClientPoll(void)
{
	int			new_socket;
	int			server_socket;
	socklen_t	client_size;
    sockaddr_in	client_addr;
	pollfd		new_poll;

	server_socket = _server_sockets[0].fd;
	new_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_size);
	if (new_socket < 0)
		return (false);
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
		return (close(new_socket) ,false);
	new_poll.fd = new_socket;
	new_poll.events = POLLIN | POLLOUT | POLLERR | POLLHUP ;
	new_poll.revents = 0;
	_server_sockets.push_back(new_poll);
	_clients.insert(std::pair<int, Client>(new_socket, Client(new_socket, client_size, client_addr)));
	std::cout << "new Client connected" << std::endl;
	return (true);
}

void	closeSocket(pollfd &pfd)
{
	if (pfd.fd > 2)
		close(pfd.fd);
}

void    sigExit(int code)
{
	(void)code;
	is_running = false;
	std::cout << "Exiting ..." << std::endl;
}

