//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER

#include "Server.hpp"

bool is_running = true;

Server::Server()
{
	/*	init pointer to function	*/
	_cmd[0] = &Server::pass;
	_cmd[1] = &Server::nick;
	_cmd[2] = &Server::user;
}

Server::~Server() {
	std::for_each(_server_sockets.begin(), _server_sockets.end(), closeSocket);
	_clients.clear();
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
	char		buffer[MAX_BUFFER];
	Message		mess;

	std::cout << "Running Server" << std::endl;
	while (is_running)
	{
		_server_sockets[0].revents = 0;
		poll_ret = poll(_server_sockets.data(), _server_sockets.size(), POLL_DELAY);
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
			if (IS_POLLIN(current_poll->revents))
			{
				memset(buffer, 0, MAX_BUFFER);
				std::cout << "new input" << std::endl;
				if (recv(current_poll->fd, buffer, MAX_BUFFER, 0) == CLOSE_SOCKET)
				{
					close(current_poll->fd);
					_server_sockets.erase(_server_sockets.begin() + i);
					std::cout << _server_sockets.size() << std::endl;
				}
				else
				{
					std::cout << "From socket " << current_poll->fd << ": " << buffer << std::endl;
					/* TEST*/
					mess.parse(buffer);
					execMessage(_clients.find(current_poll->fd)->second, mess);
				}
			}
		}
	}
}

bool	Server::initServerPoll(void)
{
	int			new_socket;
	pollfd		new_poll;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket < 0)
		return (false);
	fcntl(new_socket, F_SETFL, O_NONBLOCK);
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_iport);
	_addr.sin_addr.s_addr = INADDR_ANY;
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
	fcntl(new_socket, F_SETFL, O_NONBLOCK);
	new_poll.fd = new_socket;
	new_poll.events = POLLIN | POLLHUP;
	new_poll.revents = 0;
	_server_sockets.push_back(new_poll);
	_clients.insert(std::pair<int, Client>(new_socket, Client(new_socket, 0)));
	std::cout << _server_sockets.size() << std::endl;
	std::cout << "new Client connected" << std::endl;
	return (true);
}

void	closeSocket(pollfd pfd)
{
	close(pfd.fd);
}

void    sigExit(int code)
{
	(void)code;
	is_running = false;
	std::cout << "Exiting ..." << std::endl;
}
