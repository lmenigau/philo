/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:05:22 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/12 23:14:36 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		take_fork(t_fork *fork, int id)
{
	if (fork->ts_release == 0)
	{
		pthread_mutex_lock(&fork->lock);
		fork->ts_release = 1;
		pthread_mutex_unlock(&fork->lock);
		ex_print("%5ld %3d has taken a fork\n", id);
		return (1);
	}
	return (0);
}
