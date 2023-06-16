#include "Server.hpp"

/**
Invites a client to join a channel on the server.
@param client The client inviting.
@param message The message containing the invite command and parameters.
*/
void Server::invite(Client &client, const Message &mess) {
    if (mess.getParamNum() < 2) {
		client.reply(ERR_NEEDMOREPARAMS, "INVITE", ":Not enough parameters");
        return;
    }

    const std::string nickname = mess.getParam()[0];
    const std::string channelName = mess.getParam()[1];
	
    Channel *channel = this->getChannel(channelName);
    if (channel == NULL) {
		client.reply(ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        return;
    }
    if (!channel->isUserInChannel(*(client.getNick()))) {
		client.reply(ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        return;
    }
    if (!this->nick_in_use(nickname)) {
		client.reply(ERR_NOSUCHNICK, channelName.c_str(), ":No such nick");
        return;
    }
    if (channel->isUserInChannel(nickname)) {
		client.reply(ERR_USERONCHANNEL, channelName.c_str(), ":is already on channel");
        return;
    }
    if (channel->hasMode('i') && !channel->isUserOperator(*(client.getNick()))) {
		client.reply(ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
        return;
    }
    channel->inviteUser(nickname);
	client.reply(RPL_INVITING, nickname.c_str(), channelName.c_str());
	std::string inviteMessage = ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@" \
				+ "localhost" + " " + "INVITE" + " " + nickname + " " + channelName;
	getClient(nickname)->reply(inviteMessage.c_str());
}
