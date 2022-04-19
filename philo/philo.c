/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:05:01 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/14 02:28:53 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "philo.h"

long	micro_ts(long start)
{
	long			ts;
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	ts = tm.tv_sec * 1000000 + tm.tv_usec;
	return (ts - start);
}

void	long_write(t_mutex *lock, long *ptr, long val)
{
	pthread_mutex_lock(lock);
	*ptr = val;
	pthread_mutex_unlock(lock);
}

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
		ex_print("%5ld %3d died\n", p->info->start, p->id);
		unlock_forks(p);
		return (0);
	}
	pthread_mutex_unlock(&p->info->exit_l);
	usleep(1);
	return (1);
}

void	micro_sleep(t_philo *p, long dur)
{
	long		now;

	now = micro_ts(p->info->start);
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
		p->info->start, p->id);
	p->last_meal = micro_ts(p->info->start);
	p->ts_dead = p->last_meal + p->info->time_to_die;
	micro_sleep(p, p->info->eat_time);
	pthread_mutex_unlock(&p->left->lock);
	usleep(1);
	pthread_mutex_unlock(&p->right->lock);
	usleep(1);
}

void	think(t_philo *p)
{
	long	ttl;

	ex_print("%5ld %3d is thinking\n", p->info->start, p->id);
	ttl = p->ts_dead - micro_ts(p->info->start);
	micro_sleep(p, ttl / 2);
}

void	philosopher(t_philo *p)
{
	p->ts_dead = micro_ts(p->info->start) + p->info->time_to_die;
	if (p->id & 1)
		micro_sleep(p, 5000);
	while (check_dead(p, micro_ts(p->info->start)))
	{
		if (p->state == lock_Fork)
		{
			usleep(1);
			pthread_mutex_lock(&p->left->lock);
			pthread_mutex_lock(&p->right->lock);
		}
		else if (p->state == eating)
			eat(p);
		else if (p->state == sleeping)
		{
			if (--p->counter == 0)
				break ;
			ex_print("%5ld %3d is sleeping\n", p->info->start, p->id);
			micro_sleep(p, p->info->sleep_time);
		}
		else if (p->state == thinking)
			think(p);
		p->state = (p->state + 1) % (total_state);
	}
}
