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

long	micro_ts(void)
{
	static long		start;
	long			ts;
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	ts = tm.tv_sec * 1000000 + tm.tv_usec;
	if (!start)
		start = ts;
	return (ts - start);
}

void	long_write(t_mutex *lock, long *ptr, long val)
{
	pthread_mutex_lock(lock);
	*ptr = val;
	pthread_mutex_unlock(lock);
}

int	check_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->exit_l);
	if (philo->info->exit)
	{
		pthread_mutex_unlock(&philo->info->exit_l);
		if (philo->state == eating)
		{
			pthread_mutex_unlock(&philo->left->lock);
			pthread_mutex_unlock(&philo->right->lock);
		}
		return (0);
	}
	else if (micro_ts() > philo->ts_dead)
	{
		philo->info->exit = 1;
		pthread_mutex_unlock(&philo->info->exit_l);
		ex_print("%5ld %3d died\n", philo->id);
		if (philo->state == eating)
		{
			pthread_mutex_unlock(&philo->left->lock);
			pthread_mutex_unlock(&philo->right->lock);
		}
		return (0);
	}
	pthread_mutex_unlock(&philo->info->exit_l);
	return (1);
}

void	sleep_until(t_philo *philo, long dur)
{
	long		now;

	now = micro_ts();
	if (now + dur < philo->ts_dead)
		usleep(dur);
	else
	{
		if (philo->ts_dead - now > 0)
			usleep(philo->ts_dead - now);
		philo->alive = 0;
	}
}

void	philosopher(t_philo *philo)
{
	philo->ts_dead = micro_ts() + philo->info->time_to_die;
	//if (philo->id & 1)
	//	usleep(5000);
	while (check_dead(philo))
	{
		if (philo->state == fork1)
			take_fork(philo, &philo->left->lock);
		else if (philo->state == fork2)
			take_fork(philo, &philo->right->lock);
		else if (philo->state == eating)
		{
			ex_print("%5ld %3d is eating\n", philo->id);
			philo->ts_dead = micro_ts() + philo->info->time_to_die;
			sleep_until(philo, philo->info->eat_time);
			pthread_mutex_unlock(&philo->left->lock);
			usleep(500);
			pthread_mutex_unlock(&philo->right->lock);
		}
		else if (philo->state == sleeping)
		{
			if (--philo->counter == 0)
				break ;
			ex_print("%5ld %3d is sleeping\n", philo->id);
			sleep_until(philo, philo->info->sleep_time);
		}
		else if (philo->state == thinking)
		{
			ex_print("%5ld %3d is thinking\n", philo->id);
		}
		philo->state = (philo->state + 1) % (total_state);
	}
}
