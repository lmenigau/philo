#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_fork
{
	t_mutex		lock;
	int			taken;
}				t_fork;

typedef struct s_info {
	t_mutex		exit_l;
	long		start;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	int			maxphil;
	int			number_of_times_each_philosopher_must_eat;
	int			exit;
}				t_info;

enum e_state {
	fork1,
	fork2,
	eating,
	sleeping,
};

typedef struct s_philo
{
	pthread_t	thread;
	t_mutex		lock;
	t_fork		*forks;
	t_info		*info;
	int			state;
	int			counter;
	long		last_meal;
	int			id;
}				t_philo;

int		parse_int(char *str);
#endif
