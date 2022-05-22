/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avoid_lock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 05:26:37 by lomeniga          #+#    #+#             */
/*   Updated: 2022/05/22 20:41:35 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philo.h"

void	unlock_forks(t_philo *p)
{
	if (p->state == eating)
	{
		pthread_mutex_unlock(&p->left->lock);
		pthread_mutex_unlock(&p->right->lock);
	}
}

int	check_dead(t_philo *p)
{
	_Bool test;

	pthread_mutex_lock(&p->info->exit_l);
	test = p->info->exit;
	pthread_mutex_unlock(&p->info->exit_l);

	if (!test)
		return (1);
	unlock_forks(p);
	return (0);
}

_Bool	foreach_philo(t_info *info, t_philo *philos, int *id)
{
	int		i;
	_Bool	test;

	i = 0;
	while (i < info->maxphil)
	{
		pthread_mutex_lock(&philos[i].lock);
		test = micro_ts() > philos[i].ts_dead;
		pthread_mutex_unlock(&philos[i].lock);
		if (test)
		{
			*id = i;
			pthread_mutex_lock(&info->exit_l);
			info->exit = 1;
			pthread_mutex_unlock(&info->exit_l);
			return (0);
		}
		i++;
	}
	return (1);
}

void	monitor(t_info *info, t_philo *philos)
{
	int		id;

	while (foreach_philo(info, philos, &id))
		usleep(2000);
	ex_print("%5ld %3d has died\n", info->start, id + 1);
}

void	avoid_lock(t_philo *p)
{
	if (p->info->maxphil == 1)
	{
		ex_print("%1$5ld %2$3d has taken a fork\n", p->info->start, p->id + 1);
		micro_sleep(p, p->info->time_to_die);
	}
}
