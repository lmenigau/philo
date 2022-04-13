/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:05:22 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/13 02:34:37 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_fork(t_fork *fork, t_philo *philo)
{
	pthread_mutex_lock(&fork->lock);
	if (fork->taken == 0)
	{
		fork->taken = 1;
		pthread_mutex_unlock(&fork->lock);
		ex_print("%5ld %3d has taken a fork\n", philo->id);
		return (1);
	}
	pthread_mutex_unlock(&fork->lock);
	return (0);
}
