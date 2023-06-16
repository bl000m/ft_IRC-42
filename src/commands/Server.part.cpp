#include "Server.hpp"

void 	Server::part(Client &client, Message const &mess){
	 if (mess.getParamNum() < 1) {
        client.reply(ERR_NEEDMOREPARAMS, "PART", ":Not enough parameters");
        return;
    }
	channelNamesVec channelNames = split(mess.getParam()[0], ',');
	std::string reason = mess.getParamNum() > 1 ? mess.getParam()[1] : "No reason";
	channelNamesVecIt it;
	for (it = channelNames.begin(); it != channelNames.end(); it++){
		std::string channelName = *it;
		Channel *channel = getChannel(channelName);
		if (channel == NULL) {
        	client.reply(ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        	return;
    	}
		else if (!channel->isUserInChannel(*(client.getNick()))) {
			client.reply(ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
			return;
		}
		else{
			std::string leavingMessage = ":" + client.getFullName() + " PART " + channelName + " " + reason + "\r\n";
			if (channel->getUsersCount() > 1 && channel->isTheOnlyOperator(*(client.getNick()))){
				std::string cmd = "MODE " + channel->getName() + " +o " + channel->getOldestMemberUser(*(client.getNick()));
				Message mess;
				mess.parse(cmd);
				execMessage(client, mess);
			}
			channel->broadcastSenderIncluded(leavingMessage);
			client.removeChannel(channelName);
			channel->removeChannelUser(*(client.getNick()));
			std::cout << "user remaining in channel: " << channel->getUsersCount() << std::endl;
			if (channel->getUsersCount() == 0)
				_channels.erase(channelName);
		}
	}
}

