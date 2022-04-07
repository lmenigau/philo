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
		else if (micro_ts() - philo->last_meal > philo->info->time_to_die)
		{
			philo->info->exit = 1;
			pthread_mutex_unlock(&philo->info->exit_l);
			printf("%5ld %d died\n", micro_ts() / 1000, philo->id);
			return (0);
		}
		pthread_mutex_unlock(&philo->info->exit_l);
		return (1);
}

int		take_fork(t_philo *philo)
{
		pthread_mutex_lock(&philo->forks[philo->id].lock);
		if (philo->forks[philo->id].taken)
		{
			pthread_mutex_unlock(&philo->forks[philo->id].lock);
			return (0);
		}
		else
		{
			philo->forks[philo->id].taken = 1;
			pthread_mutex_unlock(&philo->forks[philo->id].lock);
			printf("%5ld %d has taken a fork\n", micro_ts()/1000, philo->id);
			return (1);
		}
}

int		take_fork2(t_philo *philo)
{
		pthread_mutex_lock(&philo->forks[philo->id + 1 % philo->info->maxphil].lock);
		if (philo->forks[(philo->id + 1) % philo->info->maxphil].taken)
		{
			pthread_mutex_unlock(&philo->forks[philo->id].lock);
			return (0);
		}
		else
		{
			philo->forks[(philo->id + 1) % philo->info->maxphil].taken = 1;
			pthread_mutex_unlock(&philo->forks[philo->id + 1 % philo->info->maxphil].lock);
			printf("%5ld %d has taken a fork\n", micro_ts()/1000, philo->id);
			return (1);
		}
}

void	philosopher(t_philo *philo)
{
	while (check_dead(philo))
	{
		if (philo->state == fork1 && take_fork(philo))
			philo->state = fork2;
		else if (philo->state == fork2 && take_fork2(philo))
			philo->state = eating;
		else if (philo->state == eating)
		{
			philo->last_meal = micro_ts();
			printf("%5ld %d is eating\n", micro_ts()/1000, philo->id);
			usleep(philo->info->time_to_eat);
			philo->state = sleeping;
		}
		else if (philo->state == sleeping)
		{
			printf("%5ld %d is sleeping\n", micro_ts()/1000, philo->id);
			usleep(philo->info->time_to_sleep);
		}
	}
}

#if 0
void	*philosopherold(t_philo *philo)
{
	int		i;

	i  = 0;
	while (!int_read(&philo->info->exit_l, &philo->info->exit))
	{
		if (philo->id & 1)
		{
			pthread_mutex_lock(&philo->forks[philo->id]);
			printf("%5ld %d has taken a fork\n", micro_ts()/1000, philo->id);
			pthread_mutex_lock(&philo->forks[(philo->id + 1) % philo->info->maxphil]);
			printf("%5ld %d has taken a fork\n", micro_ts()/1000, philo->id);
		}
		else
		{
			pthread_mutex_lock(&philo->forks[(philo->id + 1) % philo->info->maxphil]);
			printf("%5ld %d has taken a fork\n", micro_ts()/1000, philo->id);
			pthread_mutex_lock(&philo->forks[philo->id]);
			printf("%5ld %d has taken a fork\n", micro_ts()/1000, philo->id);
		}
		long_write(&philo->lock, &philo->last_meal, micro_ts());
		printf("%5ld %d is eating\n", micro_ts()/1000, philo->id);
		usleep(philo->info->time_to_eat);
		pthread_mutex_unlock(&philo->forks[(philo->id + 1) % philo->info->maxphil]);
		pthread_mutex_unlock(&philo->forks[philo->id]);
		printf("%5ld %d is sleeping\n", micro_ts()/1000, philo->id);
		usleep(philo->info->time_to_sleep);
		printf("%5ld %d is thinking\n", micro_ts()/1000, philo->id);
		i++;
	}
	return (NULL);
}
#endif

void	launch(t_info *info, t_philo *philos, t_fork *forks)
{
	int		i;

	i = 0;
	while (i < info->maxphil)
	{
		forks[i].taken = 0;
		pthread_mutex_init(&forks[i].lock, NULL);
		i++;
	}
	i = 0;
	while (i < info->maxphil)
	{
		philos[i].id = i;
		philos[i].forks = forks;
		philos[i].info = info;
		philos[i].last_meal = 0;
		pthread_mutex_init(&philos[i].lock, NULL);
		pthread_create(&philos[i].thread, NULL,
				(void *(*)(void *))philosopher, &philos[i]);
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
		printf("missing time_to_eat\n");
	if (ac < 5)
		printf("missing time_to_sleep\n");
	if (ac < 5)
		return (1);
	info.maxphil = parse_int(av[1]);
	info.time_to_die = parse_int(av[2]) * 1000;
	info.time_to_eat = parse_int(av[3]) * 1000;
	info.time_to_sleep = parse_int(av[4]) * 1000;
	info.start = micro_ts();
	info.exit = 0;
	forks = malloc(sizeof(*forks) * info.maxphil);
	philos = malloc(sizeof(*philos) * info.maxphil);
	launch(&info, philos, forks); 
	free(forks);
	free(philos);
}
