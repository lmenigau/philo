/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 06:51:41 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/13 02:39:15 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "philo.h"

void	ex_print(char *fmt, int id)
{
	static t_mutex	mut = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&mut);
	printf(fmt, micro_ts() / 1000, id);
	pthread_mutex_unlock(&mut);
}

t_fork	*init_forks(t_info *info)
{
	int			i;
	t_fork		*forks;

	forks = malloc(sizeof(*forks) * info->maxphil);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < info->maxphil)
	{
		forks[i].taken = 0;
		if (pthread_mutex_init(&forks[i].lock, NULL))
			return (NULL);
		i++;
	}
	return (forks);
}

void	create_philos(t_info *info, t_philo *philos, t_fork *forks)
{
	int		i;

	i = 0;
	while (i < info->maxphil)
	{
		philos[i] = (t_philo){.id = i + 1, .info = info};
		if (i & 1)
		{
			philos[i].left = &forks[i];
			philos[i].right = &forks[(i + 1) % info->maxphil];
		}
		else
		{
			philos[i].right = &forks[i];
			philos[i].left = &forks[(i + 1) % info->maxphil];
		}
		philos[i].alive = 1;
		philos[i].ts_dead = info->time_to_die;
		philos[i].state = lock1;
		philos[i].counter = info->eat_count;
		if (pthread_create(&philos[i].thread, NULL,
				(void *)philosopher, &philos[i]))
			info->maxphil = i;
		i++;
	}
}

int	wait_philos(t_info *info, t_fork *forks)
{
	t_philo		*philos;
	int			i;

	philos = malloc(sizeof(*philos) * info->maxphil);
	if (!philos)
		return (1);
	create_philos(info, philos, forks);
	i = 0;
	while (i < info->maxphil)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	free(forks);
	free(philos);
	return (0);
}

int	main(int ac, char **av)
{
	static t_info	info;

	if (arg_warn(ac))
		return (1);
	info.maxphil = parse_int(av[1]);
	info.time_to_die = parse_int(av[2]) * 1000;
	info.eat_time = parse_int(av[3]) * 1000;
	info.sleep_time = parse_int(av[4]) * 1000;
	if (ac > 5)
		info.eat_count = parse_int(av[5]);
	else
		info.eat_count = -1;
	info.start = micro_ts();
	info.exit = 0;
	return (wait_philos(&info, init_forks(&info)));
}
