/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:02:01 by lomeniga          #+#    #+#             */
/*   Updated: 2022/04/13 02:37:11 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_fork
{
	t_mutex		lock;
	long		taken;
}				t_fork;

typedef struct s_info {
	t_mutex		exit_l;
	t_fork		*forks;
	long		start;
	long		time_to_die;
	long		eat_time;
	long		sleep_time;
	int			maxphil;
	int			eat_count;
	int			exit;
}				t_info;

enum e_state {
	lock1,
	lock2,
	eating,
	sleeping,
	thinking,
	total_state
};

typedef struct s_philo
{
	pthread_t	thread;
	t_info		*info;
	t_fork		*left;
	t_fork		*right;
	t_fork		*forks;
	long		ts_dead;
	int			state;
	int			counter;
	int			id;
	int			alive;
}				t_philo;

int		parse_int(char *str);
int		arg_warn(int ac);
void	philosopher(t_philo *philo);
int		take_fork(t_fork *lock, t_philo *philo);
int		take_fork2(t_philo *philo);
long	micro_ts(void);
void	sleep_until(t_philo *philo, long ts);
void	ex_print(char *fmt, int id);
#endif
