#include "Server.hpp"

void Server::invite(Client client, const Message &message, Server *server) {
    if (message.getParamNum() < 2) {
        this->reply(client,  ERR_NEEDMOREPARAMS, "INVITE", ":Not enough parameters");
        return;
    }

    const std::string nickname = message.getParam()[0];
    const std::string channelName = message.getParam()[1];

    Channel *channel = server->getChannel(channelName);
    if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, channelName.c_str(), ":No such channel");
        return;
    }

    if (!channel->isUserInChannel(*(client.getNick()))) {
        this->reply(client,  ERR_NOTONCHANNEL, channelName.c_str(), ":You're not on that channel");
        return;
    }
	// not finished
}
