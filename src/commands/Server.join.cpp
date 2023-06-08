#include <Server.hpp>

bool	Server::createChan(std::string &name, std::string &pass, Client &client)
{
	std::string	join_message;

	Channel	chan(&client, name);
	if (chan.checkChannelName(name) == false)
		return (false);
	chan.setPassword(pass);
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
	(void)pass;

	_channels.at(name).addClient(&client);
	std::cout << "SIZE:  " << _channels.size() << std::endl;
}

void	Server::join(Client &client, Message const &mess)
{
    std::vector<std::string>	channels;
	std::vector<std::string>	keys;
	std::string					parsed;
	std::stringstream			ss;

	ss << mess.getParam()[0];
	if (mess.getParamNum() < 1)
	{
		reply(client,  ERR_NEEDMOREPARAMS, "MODE", ":Not enough parameters");
		return ;
	}
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