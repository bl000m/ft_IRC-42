#include "Server.hpp"

/**
	Kicks a user from a channel on the server.
	@param client The client performing the kick action.
	@param mess The message containing the kick command and parameters.
*/
void Server::kick(Client &client, const Message &mess) {
	/**
		Checks if the number of parameters in the kick message is sufficient
	*/
    if (mess.getParamNum() < 2) {
        this->reply(client,  ERR_NEEDMOREPARAMS, "KICK", ":Not enough parameters");
        return;
    }

    const std::string channelName = mess.getParam()[0];
    const std::string nickname = mess.getParam()[1];

	/**
		If the channel does not exist, sends an error reply to the client and returns.
	*/
    Channel *channel = this->getChannel(channelName);
    if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        return;
    }

	/**
		Checks if the client performing the kick action is a member of the channel.
	*/
    if (!channel->isUserInChannel(*(client.getNick()))) {
        this->reply(client,  ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        return;
    }

	/**
		Checks if the client performing the kick action is a channel operator.
	*/
    if (!channel->isUserOperator(*(client.getNick()))) {
        this->reply(client, ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
        return;
    }

	/**
		Checks if the kicked client is on channel.
	*/
    if (!channel->isUserInChannel(nickname)) {
        this->reply(client, ERR_USERNOTINCHANNEL, channelName.c_str(), ":They aren't on that channel");
        return;
    }

	std::string reason = mess.getParamNum() > 2 ? mess.getParam()[2] : "";

	/**
		Checks if the user to be kicked is a member of the channel.
		If not, sends an error reply to the client and returns.
		Otherwise, sends a kick message to the user and removes them from the channel.
	*/
    if (!channel->isUserInChannel(nickname)) {
		std::string NotInChanMessage = ":FT_IRC 441 " + *(client.getNick()) + " " + nickname + " "\
					 + channelName + " :They aren't on that channel\n";
		send(client.getSock(), NotInChanMessage.c_str(), NotInChanMessage.size(), 0);
        return;
    }
	else{
		Client *kickedUser = this->getClient(nickname);
		std::string kickMessage = ":" + client.getFullName() + " KICK "\
					+ channelName + " " + nickname + " " + reason + "\r\n";
		send(kickedUser->getSock(), kickMessage.c_str(), kickMessage.size(), 0);
		channel->removeChannelUser(nickname);
		channel->broadcast(kickMessage, client);
	}
}
