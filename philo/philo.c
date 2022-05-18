/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:05:01 by lomeniga          #+#    #+#             */
/*   Updated: 2022/05/18 06:44:57 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "philo.h"

long	micro_ts(void)
{
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000000 + tm.tv_usec);
}

void	micro_sleep(t_philo *p, long dur)
{
	long		now;

	now = micro_ts();
	if (now + dur < p->ts_dead)
		usleep(dur);
	else
	{
		if (p->ts_dead - now > 0)
			usleep(p->ts_dead - now);
		p->alive = 0;
	}
}

void	eat(t_philo *p)
{
	ex_print(
		"%1$5ld %2$3d has taken a fork\n"
		"%1$5ld %2$3d has taken a fork\n"
		"%1$5ld %2$3d is eating\n",
		p->info->start, p->id + 1);
	p->last_meal = micro_ts();
	p->ts_dead = p->last_meal + p->info->time_to_die;
	micro_sleep(p, p->info->eat_time);
	pthread_mutex_unlock(&p->left->lock);
	pthread_mutex_unlock(&p->right->lock);
}

void	think(t_philo *p)
{
	long	ttl;

	ex_print("%5ld %3d is thinking\n", p->info->start, p->id + 1);
	ttl = p->ts_dead - micro_ts();
	micro_sleep(p, ttl / 2);
}

void	philosopher(t_philo *p)
{
	p->ts_dead = micro_ts() + p->info->time_to_die;
	if (p->id & 1)
		micro_sleep(p, p->info->eat_time);
	avoid_lock(p);
	while (check_dead(p, micro_ts()))
	{
		if (p->state == lock_Fork)
		{
			pthread_mutex_lock(&p->left->lock);
			pthread_mutex_lock(&p->right->lock);
		}
		else if (p->state == eating)
			eat(p);
		else if (p->state == sleeping)
		{
			if (--p->counter == 0)
				break ;
			ex_print("%5ld %3d is sleeping\n", p->info->start, p->id + 1);
			micro_sleep(p, p->info->sleep_time);
		}
		else if (p->state == thinking)
			think(p);
		p->state = (p->state + 1) % (total_state);
	}
}
