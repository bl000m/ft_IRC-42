#include "Server.hpp"

/**
Invites a client to join a channel on the server.
@param client The client to be invited.
@param message The message containing the invite command and parameters.
*/
void Server::topic(Client &client, const Message &mess) {
    const std::string channelName = mess.getParam()[0];
    const std::string topic = mess.getParam()[1];
    Channel *channel = this->getChannel(channelName);

    if (mess.getParamNum() < 2) {
        if (channel->getTopic() != NULL){
            std::string topicMessage = ":FT_IRC " + RPL_TOPIC + *(client.getNick()) + " " + channelName + " " \
				+ ":No topic is set";
	        send(getClient(nickname)->getSock(), topicMessage.c_str(), topicMessage.size(), 0);
        }
        else{
            this->reply(client,  RPL_NOTOPIC, channelName, ":No topic is set");
        }
        return;
    }

	/**
	Checks if Client trying to set the topic is a channel member.
	*/
    if (!channel->isUserInChannel(*(client.getNick()))) {
        this->reply(client,  ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        return;
    }

	/**
	Checks if the number of parameters in the invite message is sufficient.
	*/
    if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
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
