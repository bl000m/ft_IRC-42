#include "Server.hpp"

static bool	isMatch(std::string name, std::string mask);

void	Server::who(Client &client, Message const &mess)
{
	std::string	target;

	if (mess.getParamNum() < 1)
		return ;
	target = mess.getParam()[0];
	if (target[0] == '#' || target[0] == '&')
	{
		who_chan(client, target);
	}
	else if (target.find_first_of("*?") == std::string::npos)
	{
		who_nick(client, target);
	}
	else
	{
		who_mask(client, target);
	}
	client.reply(RPL_ENDOFWHO, target.c_str(), ":End of WHO list");
}

void	Server::who_chan(Client &client, std::string chan_name)
{
	Channel					*chan;
	Channel::channelUsers	members;
	Channel::channelUsersIt	it;
	std::string				note;

	chan = getChan(chan_name);
	if (!chan)
		return ;
	members = chan->getChannelUsers();
	for (it = members.begin(); it != members.end(); it++)
	{
		note = who_reply(client, it->first, chan_name.c_str());
		client.reply(note.c_str());
	}
}
void	Server::who_nick(Client &client, std::string nick)
{
	std::string	note;

	note = who_reply(client, nick, NULL);
	if (!note.empty())
		client.reply(note.c_str());
}
void	Server::who_mask(Client &client, std::string mask)
{
	client_map::iterator	it;
	std::string				nick;
	std::string				note;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		nick = *(it->second.getNick());
		if (isMatch(nick, mask))
		{
			who_nick(client, nick);
		}
	}
}

std::string	Server::who_reply(Client const &client, std::string nick, char const *chan)
{
	Client			*target;
	channelListIt	it;
	std::string		chan_name = "*";
	std::string		note = "";

	target = getClient(nick);
	if (!target)
		return (note);
	if (target->isInvisible() && !share_chan(client, *target))
		return (note);
	note = note + ":localhost" + RPL_WHOREPLY + *client.getNick() + " ";
	if (chan)
		chan_name = chan;
	else
	{
		for (it = _channels.begin(); it != _channels.end(); it++)
		{
			if (it->second.isUserInChannel(nick))
			{
				chan_name = it->first;
				break ;
			}
		}
	}
	note = note + chan_name + " ";
	note = note + *target->getUser() + " <host> <server> "
		+ *target->getNick() + " <flags> :<hopcount> "
		+ *target->getHost();
	return (note);
}

/*
	string matching with wild card
	mask contain * or ?
*/
static bool	isMatch(std::string name, std::string mask)
{
	long unsigned int	i;
	long unsigned int	j;
	int	match;
	int	wildcard;

	i = 0;
	j = 0;
	match = 0;
	wildcard = -1;
	while (i < name.size())
	{
		if (j < mask.size() && (mask[j] == '?' || mask[j] == name[i]))
		{
			i++;
			j++;
		}
		else if (j < mask.size() && mask[j] == '*')
		{
			wildcard = j;
			j++;
			match = i;
		}
		else if (wildcard != -1)
		{
			j = wildcard + 1;
			match++;
			i = match;
		}
		else
			return (false);
	}
	for (; j < mask.size(); j++)
	{
		if (mask[j] != '*')
			return (false);
	}
	return (true);
}

bool	Server::share_chan(Client const &a, Client const &b)
{
	channelListIt	it;
	Channel			*chan;

	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		chan = &(it->second);
		if (chan->isUserInChannel(*a.getNick())
			&& chan->isUserInChannel(*b.getNick()))
		{
			return (true);
		}
	}
	return (false);
}