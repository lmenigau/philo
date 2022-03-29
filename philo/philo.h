#ifndef	PHILO_H
# define PHILO_H

# include <pthread.h>
typedef pthread_mutex_t t_mutex;

typedef struct	s_info
{
	t_mutex		exit_l;
	long		start;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	int			number_of_philosophers;
	int			number_of_times_each_philosopher_must_eat;
	int			exit;
}				t_info;

typedef struct	s_philo
{
	pthread_t	thread;
	t_mutex		lock;
	t_mutex		*forks;
	t_info		*info;
	long		last_meal;
	int			id;
}				t_philo;

int		parse_int(char *str);
#endif
