#include <Server.hpp>

bool	Server::createChan(std::string &name, std::string &pass, Client &client)
{
	std::string	join_message;

	Channel	chan(&client, name); //dont throw here please
	if (chan.checkChannelName(name) == false)
		return (false);
	chan.setPassword(pass);
	if (pass.size() > 0)
		chan.addMode('k');
	_channels.insert(std::pair<std::string, Channel>(name, chan));
	/*IRC*/
	join_message.append(":" + *(client).getNick() + " JOIN " + name + "\r\n");
	send(client.getSock(), join_message.c_str(), join_message.size(), 0);
	reply(client, RPL_TOPIC, "", "");
	reply(client, RPL_NAMREPLY , "", "");
	reply(client, RPL_ENDOFNAMES, "", "");
	std::cout << "SIZE:  " << _channels.size() << std::endl;
	return (true);
}

void	Server::joinChan(std::string &name, std::string &pass, Client &client)
{
	std::string	join_message;

	if (_channels.at(name).hasMode('i') && !_channels.at(name).isInvited(*(client).getNick()))
	{
		reply(client, ERR_INVITEONLYCHAN, name.c_str(), " :Cannot join channel (+i)");
		return ;
	}
	if (_channels.at(name).hasMode('l') && (int)_channels.at(name).getUsersCount() + 1 > _channels.at(name).getMemberLimit())
	{	
		reply(client, ERR_CHANNELISFULL, name.c_str(), " :Cannot join channel (+l)");
		return ;
	}
	if (_channels.at(name).hasMode('k') && pass != _channels.at(name).getPassword())
	{
		reply(client, ERR_BADCHANNELKEY, name.c_str(), " :Cannot join channel (+k)");
		return ;
	}
	_channels.at(name).addClient(&client);
	join_message.append(":" + *(client).getNick() + " JOIN " + name + "\r\n");
	send(client.getSock(), join_message.c_str(), join_message.size(), 0);
	reply(client, RPL_TOPIC, " ", _channels.at(name).getTopic().c_str());
	reply(client, RPL_NAMREPLY , "", "");
	reply(client, RPL_ENDOFNAMES, "", "");
	std::cout << "SIZE:  " << _channels.size() << std::endl;
}

void	Server::join(Client &client, Message const &mess)
{
    std::vector<std::string>	channels;
	std::vector<std::string>	keys;
	std::string					parsed;
	std::stringstream			ss;
	
	if (mess.getParamNum() < 1)
	{
		reply(client,  ERR_NEEDMOREPARAMS, "JOIN", ":Not enough parameters");
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
		/*Verify channel names and keys*/
	std::cout << channels.size() << std::endl;
	std::cout << keys.size() << std::endl;
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (keys.size() < i + 1)
			keys.push_back("");
		if (_channels.find(channels[i]) == _channels.end())
		{
			if (createChan(channels[i], keys[i], client) == false)
				std::cout << "bad name" << std::endl;
		}
		else
			joinChan(channels[i], keys[i], client);
	}
}