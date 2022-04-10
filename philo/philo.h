#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_fork
{
	t_mutex		lock;
	int			taken;
	long		ts_release;
}				t_fork;

typedef struct s_info {
	t_mutex		exit_l;
	long		start;
	long		time_to_die;
	long		eat_time;
	long		sleep_time;
	int			maxphil;
	int			number_of_times_each_philosopher_must_eat;
	volatile int	exit;
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
	t_info		*info;
	t_fork		*left;
	t_fork		*right;
	t_fork		*forks;
	long		last_meal;
	long		ts_dead;
	int			state;
	int			counter;
	int			id;
}				t_philo;

int		parse_int(char *str);
#endif
