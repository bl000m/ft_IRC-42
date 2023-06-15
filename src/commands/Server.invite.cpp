#include "Server.hpp"

/**
Invites a client to join a channel on the server.
@param client The client inviting.
@param message The message containing the invite command and parameters.
*/
void Server::invite(Client &client, const Message &mess) {
	/**
	Checks if the number of parameters in the invite message is sufficient.
	*/
    if (mess.getParamNum() < 2) {
		client.reply(ERR_NEEDMOREPARAMS, "INVITE", ":Not enough parameters");
        // this->reply(client,  ERR_NEEDMOREPARAMS, "INVITE", ":Not enough parameters");
        return;
    }

    const std::string nickname = mess.getParam()[0];
    const std::string channelName = mess.getParam()[1];

	/**
	Checks if Channel exists..
	*/
    Channel *channel = this->getChannel(channelName);
    if (channel == NULL) {
		client.reply(ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        // this->reply(client,  ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        return;
    }

	/**
	Checks if Client sending the invitation is a channel member.
	*/
    if (!channel->isUserInChannel(*(client.getNick()))) {
		client.reply(ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        // this->reply(client,  ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        return;
    }

	/**
	Checks if the client invited exist.
	*/
    if (!this->nick_in_use(nickname)) {
		client.reply(ERR_NOSUCHNICK, channelName.c_str(), ":No such nick");
        // this->reply(client, ERR_NOSUCHNICK, channelName.c_str(), ":No such nick");
        return;
    }

	/**
	Checks if the client invited is already a channel member.
	*/
    if (channel->isUserInChannel(nickname)) {
		client.reply(ERR_USERONCHANNEL, channelName.c_str(), ":is already on channel");
        // this->reply(client, ERR_USERONCHANNEL, channelName.c_str(), ":is already on channel");
        return;
    }

	/**
	Checks if the channel has the mode 'i' (invite-only) and if the client is a channel operator.
	*/
    if (channel->hasMode('i') && !channel->isUserOperator(*(client.getNick()))) {
		client.reply(ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
        // this->reply(client, ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
        return;
    }

    /**
    Adds the client to the channel invite list
	*/
    channel->inviteUser(nickname);
	/**
	Sends a reply to the sender indicating that an invite is being sent
	*/
	client.reply(RPL_INVITING, nickname.c_str(), channelName.c_str());
    // this->reply(client, RPL_INVITING, nickname.c_str(), channelName.c_str());

	/**
	Constructs the invite message and sends it to the invited client.
	*/
	std::string inviteMessage = ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@" \
				+ "localhost" + " " + "INVITE" + " " + nickname + " " + channelName;
	getClient(nickname)->reply(inviteMessage.c_str());
	// send(getClient(nickname)->getSock(), inviteMessage.c_str(), inviteMessage.size(), 0);
}
