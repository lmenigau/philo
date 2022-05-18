/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avoid_lock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 05:26:37 by lomeniga          #+#    #+#             */
/*   Updated: 2022/05/18 12:25:59 by lomeniga         ###   ########.fr       */
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

void	avoid_lock(t_philo *p)
{
	if (p->info->maxphil == 1)
	{
		ex_print("%1$5ld %2$3d has taken a fork\n", p->info->start, p->id + 1);
		micro_sleep(p, p->info->time_to_die);
	}
	if (p->info->maxphil & 1)
		micro_sleep(p, p->info->eat_time);
}
