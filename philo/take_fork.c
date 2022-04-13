/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:05:22 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/14 01:01:17 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdio.h>

long		test_fork(t_fork *fork)
{
	long	ts_release;

	pthread_mutex_lock(&fork->lock);
	ts_release = fork->ts_release;
	if (fork->ts_release == 0)
	{
		fork->ts_release = 1;
		pthread_mutex_unlock(&fork->lock);
		return (1);
	}
	pthread_mutex_unlock(&fork->lock);
	return (0);
}

int		take_fork(t_philo *philo)
{
	if (!philo->f_left && test_fork(philo->left))
	{
		philo->f_left = 1;
		ex_print("%5ld %3d has taken a fork\n", philo->id);
	}
	else if (!philo->f_right && test_fork(philo->right))
	{
		philo->f_right = 1;
		ex_print("%5ld %3d has taken a fork\n", philo->id);
	}
	else if (philo->f_right && philo->f_left)
		return (1);
	return (0);
}
