#include "Server.hpp"

/*  doubts:
- when operator PART a channel should another member be automatically set as operator?
*/
void 	Server::part(Client &client, Message const &mess){
	 if (mess.getParamNum() < 1) {
        this->reply(client,  ERR_NEEDMOREPARAMS, "PART", ":Not enough parameters");
        return;
    }
	channelNamesVec channelNames = split(mess.getParam()[0], ',');
	std::string reason = mess.getParamNum() > 1 ? mess.getParam()[1] : "No reason";
	channelNamesVecIt it;
	for (it = channelNames.begin(); it != channelNames.end(); it++){
		std::string channelName = *it;
		Channel *channel = getChannel(channelName);
		if (channel == NULL) {
        	this->reply(client,  ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        	return;
    	}
		else if (!channel->isUserInChannel(*(client.getNick()))) {
			this->reply(client,  ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
			return;
		}
		else{
			std::string leavingMessage = ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@" \
				+ "localhost" + " PART " + channelName + " " + reason + "\r\n";;
			channel->broadcastSenderIncluded(leavingMessage);
			client.removeChannel(channelName);
			channel->removeChannelUser(*(client.getNick()));
		}

	}
}

