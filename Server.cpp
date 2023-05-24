//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER

#include "Server.hpp"

Server::Server(const std::string &port, const std::string &password)
		: _port(port), _password(password), _iport(4242) {
	newPoll();
}

Server::~Server() {
	std::for_each(_server_sockets.begin(), _server_sockets.end(), closeSocket);
}

bool	Server::newPoll(void)
{
	int			new_socket;
	sockaddr_in	addr;
	pollfd		new_poll;

	new_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (new_socket < 0)
		return (false);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_iport);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(new_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
		return (close(new_socket) ,false);
	if (listen(new_socket, 42) < 0)
		return (close(new_socket) ,false);
	new_poll.fd = new_socket;
	new_poll.events = POLLIN | POLLHUP;
	_server_sockets.push_back(new_poll);
	return (true);
}

void	closeSocket(pollfd pfd)
{
	close(pfd.fd);
}