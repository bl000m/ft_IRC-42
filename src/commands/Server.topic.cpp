#include "Server.hpp"

/**
Sets a topic for a channel.
@param client The client setting the topic.
@param message The message containing the invite command and parameters.
*/
void Server::topic(Client &client, const Message &mess) {
    const std::string channelName = mess.getParam()[0];
    const std::string topic = mess.getParam()[1];
    Channel *channel = this->getChannel(channelName);

	/**
	Checks if the topic is in present in parameters. If not  the client is notified:
	- if the channel has already a topic, which topic
	- if not, topic not set
	*/
    if (mess.getParamNum() < 2) {
        if (channel->getTopic().c_str() != NULL){
            std::string topicMessage = std::string(":FT_IRC ") + RPL_TOPIC + " " + *(client.getNick()) + " " + channelName + " " \
				+ ":" + channel->getTopic().c_str();
			std::string topicWhoTimeMessage = std::string(":FT_IRC ") + RPL_TOPICWHOTIME + " " + *(client.getNick()) + " " + channelName + " " \
				+ channel->getNickCreationTopic() + " " + channel->getTimeCreationTopic();
	        send(client.getSock(), topicMessage.c_str(), topicMessage.size(), 0);
        }
        else{
            this->reply(client,  RPL_NOTOPIC, channelName.c_str(), ":No topic is set");
        }
        return;
    }

	/**
	Checks if the channel exist
	*/
    if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
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
	Checks if Client trying to set the topic for a protected topic channel has operator privileges.
	*/
	if (channel->hasMode('t') &&  !channel->isUserOperator(*(client.getNick()))) {
        this->reply(client,  ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
		return;
	}

	/**
	Creates message to be broadcasted to all the channel members when a topic is set/changed/cleared (if topic == "")
	*/
	channel->setTopic(topic, (*(client.getNick())));
	std::string changeTopicMessage = ":" + *(client.getNick()) + " " + "TOPIC" + ": " + topic + " on channel: " + channelName;
	channel->broadcastSenderIncluded(changeTopicMessage);
}
