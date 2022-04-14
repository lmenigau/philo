/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:05:22 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/14 06:05:31 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philo.h"

long	test_fork(t_fork *fork, t_philo *philo, long *ts)
{
	pthread_mutex_lock(&fork->lock);
	*ts = fork->ts_release;
	if (fork->ts_release == 0)
	{
		fork->ts_release = micro_ts() + philo->info->eat_time;
		pthread_mutex_unlock(&fork->lock);
		return (1);
	}
	pthread_mutex_unlock(&fork->lock);
	return (0);
}

int	take_fork(t_philo *philo)
{
	long		ts;

	if (!philo->f_left)
	{
		if (test_fork(philo->left, philo, &ts))
		{
			ex_print("%5ld %3d has taken a fork\n", philo->id);
			philo->f_left = 1;
		}
		else
			sleep_until(philo, ts);
	}
	if (philo->f_left && !philo->f_right)
	{
		if (test_fork(philo->right, philo, &ts))
		{
			philo->f_right = 1;
			ex_print("%5ld %3d has taken a fork\n", philo->id);
			return (1);
		}
		else
			sleep_until(philo, ts);
	}
	return (0);
}
