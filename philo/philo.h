/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomeniga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 07:02:01 by lomeniga          #+#    #+#             */
/*   Updated: 2022/05/22 16:25:04 by lomeniga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_fork
{
	t_mutex		lock;
	long		ts_release;
}				t_fork;

typedef struct s_info {
	t_mutex		exit_l;
	t_mutex		waiter;
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
	lock_Fork,
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
	t_mutex		lock;
	int			f_left;
	int			f_right;
	long		lock_time;
	long		ts_dead;
	long		last_meal;
	int			state;
	int			counter;
	int			id;
	int			alive;
}				t_philo;

int		arg_warn(int ac);
int		check_dead(t_philo *p, long now);
int		parse_int(char *str);
long	micro_ts(void);
void	avoid_lock(t_philo *p);
void	ex_print(char *fmt, long start, int id);
void	micro_sleep(t_philo *p, long dur);
void	monitor(t_info *info, t_philo *philos);
void	philosopher(t_philo *philo);
void	sleep_until(t_philo *philo, long ts);
void	take_fork(t_philo *philo, t_mutex *lok);
#endif
