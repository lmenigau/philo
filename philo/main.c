#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include "philo.h"

long	micro_ts(void)
{
	static long		start;
	long			ts;
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	ts = tm.tv_sec * 1000000 + tm.tv_usec;
	if (!start)
		start = ts;
	return (ts - start);
}

void	int_write(t_mutex *lock, int *ptr, int val)
{
	pthread_mutex_lock(lock);
	*ptr = val;
	pthread_mutex_unlock(lock);
}

int		int_read(t_mutex *lock, int *ptr)
{
	int	copy;
	pthread_mutex_lock(lock);
	copy = *ptr;
	pthread_mutex_unlock(lock);
	return (copy);
}

long	long_read(t_mutex *lock, long *ptr)
{
	long	copy;
	pthread_mutex_lock(lock);
	copy = *ptr;
	pthread_mutex_unlock(lock);
	return (copy);
}

void	long_write(t_mutex *lock, long *ptr, long val)
{
	pthread_mutex_lock(lock);
	*ptr =  val;
	pthread_mutex_unlock(lock);
}

int		check_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->exit_l);
	if (philo->info->exit)
	{
		pthread_mutex_unlock(&philo->info->exit_l);
		return (0);
	}
	else if (micro_ts() > philo->ts_dead)
	{
		philo->info->exit = 1;
		pthread_mutex_unlock(&philo->info->exit_l);
		printf("%5ld %d died\n", micro_ts() / 1000, philo->id);
		return (0);
	}
	pthread_mutex_unlock(&philo->info->exit_l);
	return (1);
}

int		test_and_set(t_mutex *lock, int *val)
{
	pthread_mutex_lock(lock);
	if (*val)
	{
		pthread_mutex_unlock(lock);
		return (0);
	}
	else
	{
		*val = 1;
		pthread_mutex_unlock(lock);
		return (1);
	}
}

void	take_fork(t_philo *philo)
{
	long ts_release;

	pthread_mutex_lock(&philo->left->lock);
	ts_release = philo->left->ts_release;
	if (ts_release > micro_ts())
	{
		pthread_mutex_unlock(&philo->left->lock);
		usleep(ts_release - micro_ts()); 
	}
	else
	{
		pthread_mutex_lock(&philo->right->lock);
		ts_release = philo->right->ts_release;
		if (ts_release > micro_ts())
		{
			pthread_mutex_unlock(&philo->left->lock);
			pthread_mutex_unlock(&philo->right->lock);
			usleep(ts_release - micro_ts()); 
		}
		else
		{
			philo->left->ts_release = micro_ts() + philo->info->eat_time;
			philo->right->ts_release = micro_ts() + philo->info->eat_time;
			pthread_mutex_unlock(&philo->left->lock);
			pthread_mutex_unlock(&philo->right->lock);
		}
	}
}

long	long_min(long a, long b)
{
	if (a < b)
		return a;
	else
		return b;
}

void	philosopher(t_philo *philo)
{
	while (check_dead(philo))
	{
		take_fork(philo);
		printf("%5ld %d has taken fork\n", micro_ts()/1000, philo->id);
		printf("%5ld %d has taken fork\n", micro_ts()/1000, philo->id);
		printf("%5ld %d is eating\n", micro_ts()/1000, philo->id);
		philo->ts_dead = micro_ts() + philo->info->time_to_die;
		long s;
		s = long_min(philo->ts_dead - micro_ts(), philo->info->eat_time);
		usleep(s);
		long_write(&philo->left->lock, &philo->left->ts_release, 0);
		long_write(&philo->right->lock, &philo->right->ts_release, 0);
		printf("%5ld %d is sleeping\n", micro_ts()/1000, philo->id);
		s = long_min(philo->ts_dead - micro_ts(), philo->info->sleep_time);
		usleep(s);
		printf("%5ld %d is thinking\n", micro_ts()/1000, philo->id);
	}
}

void	launch(t_info *info, t_philo *philos, t_fork *forks)
{
	int		i;

	i = 0;
	while (i < info->maxphil)
	{
		forks[i].ts_release = 0;
		pthread_mutex_init(&forks[i].lock, NULL);
		i++;
	}
	i = 0;
	while (i < info->maxphil)
	{
		philos[i].id = i;
		philos[i].info = info;
		if (i & 1)
		{
			philos[i].left = &forks[i];
			philos[i].right = &forks[(i + 1) % info->maxphil];
		}
		else
		{
			philos[i].right = &forks[i];
			philos[i].left = &forks[(i + 1) % info->maxphil];
		}
		philos[i].last_meal = 0;
		philos[i].ts_dead = info->time_to_die;
		philos[i].state = fork1;
		pthread_create(&philos[i].thread, NULL,
				(void *)philosopher, &philos[i]);
		i++;
	}
	i = 0;
	while (i < info->maxphil)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

int		main(int ac, char **av)
{
	static t_info	info;
	t_philo			*philos;
	t_fork			*forks;

	if (ac < 2)
		printf("missing maxphil\n");
	if (ac < 3)
		printf("missing time_to_die\n");
	if (ac < 4)
		printf("missing eat_time\n");
	if (ac < 5)
		printf("missing sleep_time\n");
	if (ac < 5)
		return (1);
	info.maxphil = parse_int(av[1]);
	info.time_to_die = parse_int(av[2]) * 1000;
	info.eat_time = parse_int(av[3]) * 1000;
	info.sleep_time = parse_int(av[4]) * 1000;
	info.start = micro_ts();
	info.exit = 0;
	forks = malloc(sizeof(*forks) * info.maxphil);
	philos = malloc(sizeof(*philos) * info.maxphil);
	launch(&info, philos, forks);
	free(forks);
	free(philos);
}
