#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

/*
	args: <ip> <port> <pass>
*/
#define PORT 12000
 
int main(void)
{
    int			sockfd;
    sockaddr_in	servadd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	{
		std::cerr << "Cannot create socket" << std::endl;
        return (0);
    }
    bzero(&servadd, sizeof(servadd));
    servadd.sin_family = AF_INET;
    servadd.sin_addr.s_addr = inet_addr("127.0.0.1");
    servadd.sin_port = htons(PORT);
    if (connect(sockfd, (sockaddr*)&servadd, sizeof(servadd)) != 0)
	{
		std::cerr << 
			"Cannot connect to the server\nMake sure the server is running on or the port/ip is correct"
				<< std::endl;
        return (0);
    }
    std::cout << "Connected on localhost:4444" << std::endl;
	std::cout << "Listing" << std::endl;
    while (1)
	{}
    close(sockfd);
	return (0);
}