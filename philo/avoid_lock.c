/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avoid_lock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 05:26:37 by lomeniga          #+#    #+#             */
/*   Updated: 2022/05/22 16:24:12 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlock_forks(t_philo *p)
{
	if (p->state == eating)
	{
		pthread_mutex_unlock(&p->left->lock);
		pthread_mutex_unlock(&p->right->lock);
	}
}

int	check_dead(t_philo *p, long now)
{
	pthread_mutex_lock(&p->info->exit_l);
	if (p->info->exit)
	{
		pthread_mutex_unlock(&p->info->exit_l);
		unlock_forks(p);
		return (0);
	}
	else if (now > p->ts_dead)
	{
		p->info->exit = 1;
		pthread_mutex_unlock(&p->info->exit_l);
		ex_print("%5ld %3d died\n", p->info->start, p->id + 1);
		unlock_forks(p);
		return (0);
	}
	pthread_mutex_unlock(&p->info->exit_l);
	return (1);
}

void	monitor(t_info *info, t_philo *philos)
{
	int		i;
	_Bool	flag;

	flag = 1;
	while (flag)
	{
		i = 0;
		while (flag && i < info->maxphil)
		{
			pthread_mutex_lock(&philos[i].lock);
			if (micro_ts() > philos[i].ts_dead)
			{
				pthread_mutex_lock(&info->exit_l);
				info->exit = 1;
				flag = 0; 
			}
			pthread_mutex_unlock(&philos[i].lock);
			i++;
		}
		ex_print("%6ld %3d died\n", info->start, i + 1);
	}
}

void	avoid_lock(t_philo *p)
{
	if (p->info->maxphil == 1)
	{
		ex_print("%1$5ld %2$3d has taken a fork\n", p->info->start, p->id + 1);
		micro_sleep(p, p->info->time_to_die);
	}
}
