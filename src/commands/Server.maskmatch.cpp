#include "Server.hpp"

/*
	string matching with wild card
	mask contain * or ?
*/
bool	isMatch(std::string name, std::string mask)
{
	int	i;
	int	j;
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
