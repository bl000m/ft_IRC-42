#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

/*
	args: <ip> <port> <pass>
*/
#define PORT 16000
#define IP	 "127.0.0.1"

void connectToIrc(char *pass, int &sockfd)
{
	std::string	to_send;
	char		buff[512];

	to_send = ("PASS " + std::string(pass) + "\r\n");
	//to_send.append("NICK ircBot\r\n");
	//to_send.append("USER ircBot 0 * bot\r\n");
	send(sockfd, to_send.c_str(), to_send.size(), 0);
	bzero(buff, 512);
	std::cout << "read serv" << std::endl;
	read(sockfd, buff, 512);
	std::cout << "serv" << buff << std::endl;
}

int main(int argc, char **argv)
{
    int			sockfd;
    sockaddr_in	servadd;

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
	connectToIrc(argv[1], sockfd);
	std::cout << "ok" << std::endl;
    while (1)
	{}
    close(sockfd);
	return (0);
}