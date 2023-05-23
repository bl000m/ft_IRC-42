//  NO 42 HEADER 
//  NO 42 HEADER 
//  NO 42 HEADER

#ifndef SERVER_HPP
# define SERVER_HPP

#include <stdexcept>
#include <iostream>
#include <stdexcept>

class Server {

    public:
        Server(const std::string &port, const std::string &password);
        ~Server();

    private:
        std::string _port;
        std::string _password;

};

#endif