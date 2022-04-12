/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:04:19 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/12 07:04:19 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdio.h>

int	arg_warn(int ac)
{
	if (ac < 2)
		printf("missing maxphil\n");
	if (ac < 3)
		printf("missing time_to_die\n");
	if (ac < 4)
		printf("missing eat_time\n");
	if (ac < 5)
		printf("missing sleep_time\n");
	if (ac > 6)
		printf("eror: maximum 5 argument\n");
	if (ac < 5 || ac > 6)
		return (1);
	return (0);
}

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
