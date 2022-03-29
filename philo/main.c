#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include "philo.h"

long	micro_ts(void)
{
	struct timeval tm;	

	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000000 + tm.tv_usec);
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

void	*philosopher(t_philo *philo)
{
	int		i;

	i  = 0;
	while (!int_read(&philo->info->exit_l, &philo->info->exit))
	{
		if (philo->id & 1)
		{
			pthread_mutex_lock(&philo->forks[philo->id]);
			printf("%ld %d has taken a fork\n", micro_ts()/1000, philo->id);
			pthread_mutex_lock(&philo->forks[(philo->id + 1) % philo->info->number_of_philosophers]);
			printf("%ld %d has taken a fork\n", micro_ts()/1000, philo->id);
		}
		else
		{
			pthread_mutex_lock(&philo->forks[(philo->id + 1) % philo->info->number_of_philosophers]);
			printf("%ld %d has taken a fork\n", micro_ts()/1000, philo->id);
			pthread_mutex_lock(&philo->forks[philo->id]);
			printf("%ld %d has taken a fork\n", micro_ts()/1000, philo->id);
		}
		long_write(&philo->lock, &philo->last_meal, micro_ts());
		printf("%ld %d is eating\n", micro_ts()/1000, philo->id);
		usleep(philo->info->time_to_eat);
		pthread_mutex_unlock(&philo->forks[(philo->id + 1) % philo->info->number_of_philosophers]);
		pthread_mutex_unlock(&philo->forks[philo->id]);
		printf("%ld %d is sleeping\n", micro_ts()/1000, philo->id);
		usleep(philo->info->time_to_sleep);
		printf("%ld %d is thinking\n", micro_ts()/1000, philo->id);
		i++;
	}
	return (NULL);
}

void	monitor(t_info *info, t_philo *philos)
{
	int		i;
	int		loop;

	while (1)
	{
		i = 0;
		while (i < info->number_of_philosophers)
		{
			if (micro_ts() - long_read(&philos[i].lock, &philos[i].last_meal) > info->time_to_die)
			{
				printf("%ld %d has died\n", micro_ts() / 1000, philos[i].id);
				return ;
			}
			i++;
		}
	}
}

void	launch(t_info *info, t_philo *philos, t_mutex *forks)
{
	int		i;

	i = 0;
	while (i < info->number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < info->number_of_philosophers)
	{
		philos[i].id = i;
		philos[i].forks = forks;
		philos[i].info = info;
		philos[i].last_meal = micro_ts();
		pthread_mutex_init(&philos[i].lock, NULL);
		pthread_create(&philos[i].thread, NULL,
				(void *(*)(void *))philosopher, &philos[i]);
		i++;
	}
	i = 0;
	monitor(info, philos);
	int_write(&info->exit_l, &info->exit, 1);
	while (i < info->number_of_philosophers)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

int		main(int ac, char **av)
{
	static t_info	info;
	t_philo			*philos;
	t_mutex			*forks;

	if (ac < 2)
		printf("missing number_of_philosophers\n");
	if (ac < 3)
		printf("missing time_to_die\n");
	if (ac < 4)
		printf("missing time_to_eat\n");
	if (ac < 5)
		printf("missing time_to_sleep\n");
	if (ac < 5)
		return (1);
	info.number_of_philosophers = parse_int(av[1]);
	info.time_to_die = parse_int(av[2]) * 1000;
	info.time_to_eat = parse_int(av[3]) * 1000;
	info.time_to_sleep = parse_int(av[4]) * 1000;
	info.start = micro_ts();
	forks = malloc(sizeof(*forks) * info.number_of_philosophers);
	philos = malloc(sizeof(*philos) * info.number_of_philosophers);
	launch(&info, philos, forks); 
	free(forks);
	free(philos);
}
