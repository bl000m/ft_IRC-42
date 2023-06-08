#include "Server.hpp"

/**

*/
void Server::kick(Client &client, const Message &mess) {
    if (mess.getParamNum() < 2) {
        this->reply(client,  ERR_NEEDMOREPARAMS, "KICK", ":Not enough parameters");
        return;
    }

    const std::string channelName = mess.getParam()[0];
    const std::string nickname = mess.getParam()[1];

	/**
	*/
    Channel *channel = this->getChannel(channelName);
    if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        return;
    }

	/**
	*/
    if (!channel->isUserInChannel(*(client.getNick()))) {
        this->reply(client,  ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        return;
    }



	/**
	*/
    if (!channel->isUserOperator(*(client.getNick()))) {
        this->reply(client, ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
        return;
    }

	std::string reason = mess.getParamNum().size() > 2 ? mess.getParam()[2] : "";

	/**
	*/
    if (!channel->isUserInChannel(nickname)) {
		std::string NotInChanMessage = ":FT_IRC 441 " + *(client.getNick()) + " " + nickname + " "\
					 + channelName + " :They aren't on that channel";
		send(client.getSock(), NotInChanMessage.c_str(), NotInChanMessage.size(), 0);
        return;
    }
	else{
		// send kicked client a notif
		channel->removeChannelUser(nickname);
		// erase channel
		std::string kickMessage = "kick meassage to be written";
		channel->broadcast(kickMessage, *(this->getClient(nickname)));
	}
}
