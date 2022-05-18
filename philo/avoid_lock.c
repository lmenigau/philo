/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avoid_lock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 05:26:37 by lomeniga          #+#    #+#             */
/*   Updated: 2022/05/18 05:26:38 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	avoid_lock(t_philo *p)
{
	if (p->info->maxphil == 1)
	{
		ex_print("%1$5ld %2$3d has taken a fork\n", p->info->start, p->id + 1);
		micro_sleep(p, p->info->time_to_die);
	}
}
