#include "Server.hpp"

Channel *Server::getChannel(const std::string &channelName){
	channelListIt it;
	for (it = _channels.begin(); it != _channels.end(); it++){
		if (it->first == channelName)
			return &(it->second);
	}
	return NULL;
}
