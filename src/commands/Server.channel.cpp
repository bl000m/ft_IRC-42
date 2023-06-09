#include "Server.hpp"

Channel *Server::getChannel(const std::string &channelName){
	channelListIt it;
	it = _channels.find(channelName);
	if (it->first == channelName)
		return &(it->second);
	return NULL;
}
