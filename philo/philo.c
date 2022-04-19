/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
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

void	unlock_forks(t_philo *philo)
{
	if (philo->state == eating)
	{
		pthread_mutex_unlock(&philo->left->lock);
		pthread_mutex_unlock(&philo->right->lock);
	}
}

int	check_dead(t_philo *philo, long now)
{
	pthread_mutex_lock(&philo->info->exit_l);
	if (philo->info->exit)
	{
		pthread_mutex_unlock(&philo->info->exit_l);
		unlock_forks(philo);
		return (0);
	}
	else if (now > philo->ts_dead)
	{
		philo->info->exit = 1;
		pthread_mutex_unlock(&philo->info->exit_l);
		ex_print("%5ld %3d died\n", philo->info->start, philo->id);
		unlock_forks(philo);
		return (0);
	}
	pthread_mutex_unlock(&philo->info->exit_l);
	usleep(1);
	return (1);
}

void	sleep_until(t_philo *philo, long dur)
{
	long		now;

	now = micro_ts(philo->info->start);
	if (now + dur < philo->ts_dead)
		usleep(dur);
	else
	{
		if (philo->ts_dead - now > 0)
			usleep(philo->ts_dead - now);
		philo->alive = 0;
	}
}

void	eat(t_philo *philo)
{
	ex_print(
		"%1$5ld %2$3d has taken a fork\n"
		"%1$5ld %2$3d has taken a fork\n"
		"%1$5ld %2$3d is eating\n",
		philo->info->start, philo->id);
	philo->last_meal = micro_ts(philo->info->start);
	philo->ts_dead = philo->last_meal + philo->info->time_to_die;
	sleep_until(philo, philo->info->eat_time);
	pthread_mutex_unlock(&philo->left->lock);
	usleep(1);
	pthread_mutex_unlock(&philo->right->lock);
	usleep(1);
}

void	think(t_philo *philo)
{
	long	ttl;

	ex_print("%5ld %3d is thinking\n", philo->info->start, philo->id);
	ttl = philo->ts_dead - micro_ts(philo->info->start);
	sleep_until(philo, ttl / 2);
}

void	philosopher(t_philo *philo)
{
	philo->ts_dead = micro_ts(philo->info->start) + philo->info->time_to_die;
	if (philo->id & 1)
		sleep_until(philo, 5000);
	while (check_dead(philo, micro_ts(philo->info->start)))
	{
		if (philo->state == lock_Fork)
		{
			usleep(1);
			pthread_mutex_lock(&philo->left->lock);
			pthread_mutex_lock(&philo->right->lock);
		}
		else if (philo->state == eating)
			eat(philo);
		else if (philo->state == sleeping)
		{
			if (--philo->counter == 0)
				break ;
			ex_print("%5ld %3d is sleeping\n", philo->info->start, philo->id);
			sleep_until(philo, philo->info->sleep_time);
		}
		else if (philo->state == thinking)
			think(philo);
		philo->state = (philo->state + 1) % (total_state);
	}
}
