#include "Server.hpp"

/**
Sets a topic for a channel.
@param client The client setting the topic.
@param message The message containing the invite command and parameters.
*/
void Server::topic(Client &client, const Message &mess) {
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

    if (mess.getParamNum() < 2) {
        if (channel->getTopic().empty()){
            this->reply(client,  RPL_NOTOPIC, channelName.c_str(), ":No topic is set");
        }
        else{
            std::string topicMessage = std::string(":FT_IRC ") + RPL_TOPIC + " " + *(client.getNick()) + " " + channelName + " " \
				+ ":" + channel->getTopic().c_str() + "\r\n";
			std::string topicWhoTimeMessage = std::string(":FT_IRC ") + RPL_TOPICWHOTIME + " " + *(client.getNick()) + " " + channelName + " " \
				+ channel->getNickCreationTopic() + " " + channel->getTimeCreationTopic() + "\r\n";
	        client.reply(topicMessage.c_str());
	        client.reply(topicWhoTimeMessage.c_str());
        }
        return;
    }

    std::string topic = mess.getParam()[1];

    if (channel == NULL) {
		client.reply(ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        return;
    }

	if (channel->hasMode('t') &&  !channel->isUserOperator(*(client.getNick()))) {
		client.reply(ERR_CHANOPRIVSNEEDED, channelName.c_str(), ":You're not channel operator");
		return;
	}

	channel->setTopic(topic, (*(client.getNick())));
	std::string changeTopicMessage = ":" + *(client.getNick()) + " TOPIC " + channelName + " :" + topic + "\r\n";
	channel->broadcastSenderIncluded(changeTopicMessage);
}
