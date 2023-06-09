#include "Server.hpp"

/**
Invites a client to join a channel on the server.
@param client The client inviting.
@param message The message containing the invite command and parameters.
*/
void Server::invite(Client &client, const Message &mess) {
    if (mess.getParamNum() < 2) {
        this->reply(client,  ERR_NEEDMOREPARAMS, "INVITE", ":Not enough parameters");
        return;
    }

    const std::string nickname = mess.getParam()[0];
    const std::string channelName = mess.getParam()[1];

	/**
	Checks if the number of parameters in the invite message is sufficient.
	*/
    Channel *channel = this->getChannel(channelName);
    if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        return;
    }

	/**
	Checks if Client sending the invitation is a channel member.
	*/
    if (!channel->isUserInChannel(*(client.getNick()))) {
        this->reply(client,  ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        return;
    }

	/**
	Checks if the client invited is already a channel member.
	*/
    if (channel->isUserInChannel(nickname)) {
        this->reply(client, ERR_USERONCHANNEL, channelName.c_str(), ":is already on channel");
        return;
    }

	/**
	Checks if the channel has the mode 'i' (invite-only) and if the client is a channel operator.
	*/
    if (channel->hasMode('i') && !channel->isUserOperator(*(client.getNick()))) {
        this->reply(client, ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
        return;
    }

    /**
    Adds the client to the channel invite list
	*/
    channel->inviteUser(nickname);
	/**
	Sends a reply to the sender indicating that an invite is being sent
	*/
    this->reply(client, RPL_INVITING, nickname.c_str(), channelName.c_str());

	/**
	Constructs the invite message and sends it to the invited client.
	*/
	std::string inviteMessage = ":" + *(client.getNick()) + "!" + *(client.getUser()) + "@" \
				+ "localhost" + " " + "INVITE" + nickname + " " + channelName;
	send(getClient(nickname)->getSock(), inviteMessage.c_str(), inviteMessage.size(), 0);
}
