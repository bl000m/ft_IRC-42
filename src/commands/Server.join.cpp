#include <Server.hpp>

bool	Server::createChan(std::string &name, std::string &pass, Client &client)
{
	std::string	join_message;
	std::string	namreply_message;

	Channel	chan(&client, name);
	if (chan.checkChannelName(name) == false)
		return (false);
	chan.setPassword(pass);
	if (pass.size() > 0)
		chan.addMode('k');
	_channels.insert(std::pair<std::string, Channel>(name, chan));
	client.addChannel(&_channels.at(name), name);
	/*IRC*/
	join_message.append(":" + *(client).getNick() + " JOIN " + name );
	namreply_message.append("= " + name);
	client.reply(join_message.c_str());
	client.reply(RPL_TOPIC, name.c_str(), "");
	client.reply(RPL_NAMREPLY , namreply_message.c_str(), chan.getClientList().c_str());
	client.reply(RPL_ENDOFNAMES, name.c_str(), "");
	// send(client.getSock(), join_message.c_str(), join_message.size(), 0);
	// reply(client, RPL_TOPIC, name.c_str(), "");
	// reply(client, RPL_NAMREPLY , namreply_message.c_str(), chan.getClientList().c_str());
	// reply(client, RPL_ENDOFNAMES, name.c_str(), "");
	return (true);
}

void	Server::joinChan(std::string &name, std::string &pass, Client &client)
{
	std::string	join_message;
	std::string	namreply_message;

	if (_channels.at(name).hasMode('i') && !_channels.at(name).isInvited(*(client).getNick()))
	{
		client.reply(ERR_INVITEONLYCHAN, name.c_str(), " :Cannot join channel (+i)");
		// reply(client, ERR_INVITEONLYCHAN, name.c_str(), " :Cannot join channel (+i)");
		return ;
	}
	if (_channels.at(name).hasMode('l') && (int)_channels.at(name).getUsersCount() >= _channels.at(name).getMemberLimit())
	{
		client.reply(ERR_CHANNELISFULL, name.c_str(), " :Cannot join channel (+l)");
		// reply(client, ERR_CHANNELISFULL, name.c_str(), " :Cannot join channel (+l)");
		return ;
	}
	if (_channels.at(name).hasMode('k') && pass != _channels.at(name).getPassword())
	{
		client.reply(ERR_BADCHANNELKEY, name.c_str(), " :Cannot join channel (+k)");
		// reply(client, ERR_BADCHANNELKEY, name.c_str(), " :Cannot join channel (+k)");
		return ;
	}
	_channels.at(name).addClient(&client);
	client.addChannel(&_channels.at(name), name);
	join_message.append(":" + *(client).getNick() + " JOIN " + name + "\r\n");
	namreply_message.append("= " + name);
	client.reply(join_message.c_str());
	client.reply(RPL_TOPIC, name.c_str(), _channels.at(name).getTopic().c_str());
	client.reply(RPL_NAMREPLY , namreply_message.c_str(), _channels.at(name).getClientList().c_str());
	client.reply(RPL_ENDOFNAMES, name.c_str(), "");
	// send(client.getSock(), join_message.c_str(), join_message.size(), 0);
	// reply(client, RPL_TOPIC, name.c_str(), _channels.at(name).getTopic().c_str());
	// reply(client, RPL_NAMREPLY , namreply_message.c_str(), _channels.at(name).getClientList().c_str());
	// reply(client, RPL_ENDOFNAMES, name.c_str(), "");
	_channels.at(name).broadcast(join_message, client);
}

void	Server::join(Client &client, Message const &mess)
{
    std::vector<std::string>	channels;
	std::vector<std::string>	keys;
	std::string					parsed;
	std::stringstream			ss;

	if (mess.getParamNum() < 1)
	{
		client.reply(ERR_NEEDMOREPARAMS, "JOIN", ":Not enough parameters");
		// reply(client,  ERR_NEEDMOREPARAMS, "JOIN", ":Not enough parameters");
		return ;
	}
	ss << mess.getParam()[0];
	while (getline(ss, parsed, ','))
		channels.push_back(parsed);
	if (mess.getParamNum() >= 2)
	{
		ss.clear();
		ss << mess.getParam()[1];
		while (getline(ss, parsed, ','))
			keys.push_back(parsed);
	}
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (keys.size() < i + 1)
			keys.push_back("");
		if (_channels.find(channels[i]) == _channels.end())
		{
			if (createChan(channels[i], keys[i], client) == false)
				client.reply(ERR_BADCHANMASK, channels[i].c_str(), " :Bad Channel Mask");
				// reply(client, ERR_BADCHANMASK, channels[i].c_str(), " :Bad Channel Mask");
		}
		else
			joinChan(channels[i], keys[i], client);
	}
}
