#include "Server.hpp"

/**
	Kicks a user from a channel on the server.
	@param client The client performing the kick action.
	@param mess The message containing the kick command and parameters.
*/
void Server::kick(Client &client, const Message &mess) {
    if (mess.getParamNum() < 2) {
		client.reply(ERR_NEEDMOREPARAMS, "KICK", ":Not enough parameters");
        return;
    }

    const std::string channelName = mess.getParam()[0];

    Channel *channel = this->getChannel(channelName);
    if (channel == NULL) {
		client.reply(ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        return;
    }
    if (!channel->isUserInChannel(*(client.getNick()))) {
		client.reply(ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        return;
    }
    if (!channel->isUserOperator(*(client.getNick()))) {
		client.reply(ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
        return;
    }

	std::string reason = mess.getParamNum() > 2 ? mess.getParam()[2] : "";

    std::stringstream					ss;
	std::string							nick;
	std::vector<std::string>			victims;
	std::vector<std::string>::iterator	it;

	ss << mess.getParam()[1];
	while (getline(ss, nick, ','))
		victims.push_back(nick);
	for (it = victims.begin(); it != victims.end(); it++)
	{
		/**
			Checks if the user to be kicked is a member of the channel.
			If not, sends an error reply to the client and returns.
			Otherwise, sends a kick message to the user and removes them from the channel.
		*/
		if (!channel->isUserInChannel(*it)) {
			client.reply(ERR_USERNOTINCHANNEL, channelName.c_str(), ":They aren't on that channel");
			return;
		}
		else{
			Client *kickedUser = this->getClient(*it);
			std::string kickMessage = ":" + client.getFullName() + " KICK "\
						+ channelName + " " + *it + " " + reason + "\r\n";
			kickedUser->reply(kickMessage.c_str());
			channel->removeChannelUser(*it);
			channel->broadcastSenderIncluded(kickMessage);
		}
	}
}
