#include <limits.h>

_Bool	is_num(char c)
{
	return (c >= '0' && c <= '9');
}

_Bool	is_space(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

int	parse_int(char *str)
{
	long	res;
	int		i;

	i = 0;
	res = 0;
	while (str[i] && is_num(str[i]))
	{
		res = res * 10 + (str[i] - '0');
		if (res > INT_MAX || res < INT_MIN)
			return (0);
		(i)++;
	}
	return (res);
}
