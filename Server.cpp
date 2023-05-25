//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER

#include "Server.hpp"

Server::Server(const std::string &port, const std::string &password)
		: _port(port), _password(password), _iport(6969) {
	initServerPoll();
	std::cout << "Socket init" << std::endl;
}

Server::~Server() {
	std::for_each(_server_sockets.begin(), _server_sockets.end(), closeSocket);
}

void	Server::run(void)
{
	int		poll_ret;
	pollfd	server_poll;

	std::cout << "Running Starting" << std::endl;
	server_poll = _server_sockets.data()[0];
	while (1)
	{
		poll_ret = poll(_server_sockets.data(), _server_sockets.size(), POLL_DELAY);
		if (poll_ret < 0)
			perror("poll()");
		if (!(server_poll.revents & POLLIN))
			newClientPoll();
	}
}

bool	Server::initServerPoll(void)
{
	int			new_socket;
	pollfd		new_poll;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket < 0)
		return (false);
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_iport);
	_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(new_socket, reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr)) < 0)
		return (close(new_socket) ,false);
	if (listen(new_socket, MAX_QUEUE_CONNECTION) < 0)
		return (close(new_socket) ,false);
	new_poll.fd = new_socket;
	new_poll.events = POLLIN | POLLHUP;
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

	server_socket = _server_sockets.data()[0].fd;
	new_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_size);
	if (new_socket < 0)
		return (false);
	new_poll.fd = new_socket;
	new_poll.events = POLLIN | POLLHUP;
	new_poll.revents = 0;
	_server_sockets.push_back(new_poll);
	std::cout << "new Client connected" << std::endl;
	return (true);
}

void	closeSocket(pollfd pfd)
{
	close(pfd.fd);
}