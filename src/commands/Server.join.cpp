#include <Server.hpp>

void	Server::createChan(std::string &name, std::string &pass, Client &client)
{
	Channel	chan(name, pass, &client);
	_channels.insert(std::pair<std::string, Channel>(name, chan));
}

void	Server::checkChan(std::string &name)
{
	(void)name;
}

void	joinChan(std::string &name, std::string &pass, Client *client)
{
	(void)name;
	(void)pass;
	(void)client;
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
	keys.reserve(channels.size());
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
		createChan(channels[i], keys[i], client);
	}
}