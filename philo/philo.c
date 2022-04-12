/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:05:01 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/12 07:05:10 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
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
		return (0);
	}
	else if (philo->alive == 0)
	{
		philo->info->exit = 1;
		pthread_mutex_unlock(&philo->info->exit_l);
		printf("%5ld %3d died\n", micro_ts() / 1000, philo->id);
		return (0);
	}
	pthread_mutex_unlock(&philo->info->exit_l);
	return (1);
}

void	sleep_until(t_philo *philo, long ts)
{
	long		now;

	now = micro_ts();
	if (ts < philo->ts_dead)
	{
		if (ts - now > 0)
			usleep(ts - now);
	}
	else
	{
		if (philo->ts_dead - now > 0)
			usleep(philo->ts_dead - now);
		philo->alive = 0;
	}
}

void	philosopher(t_philo *philo)
{
	while (check_dead(philo))
	{
		if (philo->state == hungry)
			take_fork(philo);
		else if (philo->state == eating)
		{
			printf("%1$5ld %2$3d has taken fork\n%1$5ld %2$3d has taken fork\n"
				"%1$5ld %2$3d is eating\n", micro_ts() / 1000, philo->id);
			philo->ts_dead = micro_ts() + philo->info->time_to_die;
			sleep_until(philo, micro_ts() + philo->info->eat_time);
		}
		else if (philo->state == sleeping)
		{
			long_write(&philo->left->lock, &philo->left->ts_release, 0);
			long_write(&philo->right->lock, &philo->right->ts_release, 0);
			if (++philo->counter == philo->info->eat_count)
				break ;
			printf("%5ld %3d is sleeping\n", micro_ts() / 1000, philo->id);
			sleep_until(philo, micro_ts() + philo->info->sleep_time);
		}
		else if (philo->state == thinking)
			printf("%5ld %3d is thinking\n", micro_ts() / 1000, philo->id);
		philo->state = (philo->state + 1) % (total_state);
	}
}
