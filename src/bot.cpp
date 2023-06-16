#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/*
	args: <ip> <port> <pass>
*/
#define PORT 4444
#define MAXBUFFER  512
#define IP	 "127.0.0.1"

bool doCommand(int socket, std::string command)
{
	char		rbuff[MAXBUFFER];
	int			read;

	bzero(rbuff, sizeof(rbuff));
	send(socket, command.c_str(), command.size(), 0);
	read = recv(socket, rbuff, MAXBUFFER, MSG_DONTWAIT);
	if (read >= 0)
	{
		std::cout << "from serv: " << rbuff << std::endl;
		return (false);
	}
	return (true);
}

void checkCommand(int socket, char *buff)
{
	std::string b = buff;

	if (b.find("INVITE") != std::string::npos)
	{
		ssize_t	pos = b.find("#");
		std::cout << "Lets join " << b.substr(pos) << std::endl;
		std::string command;
		command.append("JOIN " + b.substr(pos) + "\r\n");
		send(socket, command.c_str(), command.size(), 0);
	}
	if (b.find("!ping") != std::string::npos)
	{
		std::string command;
		command.append("PRIVMSG " + b.substr(b.find("#"), b.size() - b.find("ping")) + " Pong\r\n");
		send(socket, command.c_str(), command.size(), 0);
	}
}

int main(int argc, char **argv)
{
    int			sockfd;
    sockaddr_in	servadd;
	bool		regi = false;
	char		buff[MAXBUFFER];
	int			read_size;

	(void)argv;
	if (argc < 2)
	{
		std::cerr << "args: <pass> <port> [ip]" << std::endl;
        return (0);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	{
		std::cerr << "Cannot create socket" << std::endl;
        return (0);
    }
    bzero(&servadd, sizeof(servadd));
    servadd.sin_family = AF_INET;
    servadd.sin_addr.s_addr = inet_addr(IP);
    servadd.sin_port = htons(PORT);
    if (connect(sockfd, (sockaddr*)&servadd, sizeof(servadd)) != 0)
	{
		std::cerr << 
			"Cannot connect to the server\nMake sure the server is running on or the port/ip is correct"
				<< std::endl;
		close(sockfd);
        return (0);
    }
    std::cout << "Connected on localhost:4444" << std::endl;
	std::cout << "Listing" << std::endl;
    while (1)
	{
		bzero(buff, sizeof(buff));
		while (regi == false)
		{
			std::cout << "Try to register" << std::endl;
			if (doCommand(sockfd, std::string("PASS ") + argv[1] + "\r\n") == false)
				break;
			sleep(1);
			if (doCommand(sockfd, std::string("NICK ircbot\r\n")) == false)
				break;
			sleep(1);
			if (doCommand(sockfd, std::string("USER ircbot * 0 bot\r\n")) == false)
				break;
			sleep(1);
			std::cout << "Register success" << std::endl;
			regi = true;
		}
		if (regi == false)
		{
			std::cerr << "Register failed exiting ..." << std::endl;
			close(sockfd);
			return (0);
		}
		read_size = recv(sockfd, buff, MAXBUFFER, MSG_DONTWAIT);
		if (read_size > 0)
		{
			std::cout << "from serv: " << buff << std::endl;
			checkCommand(sockfd, buff);
		}
		sleep(1);
	}
    close(sockfd);
	return (0);
}