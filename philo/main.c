#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "philo.h"

long	micro_ts(void)
{
	struct timeval tm;	

	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000000 + tm.tv_usec);
}

void	*philosopher(t_philo *philo)
{
	printf("%ld %d is thinking\n", micro_ts(), philo->id);

	usleep(philo->info->time_to_sleep);
	return (NULL);
}

void	launch(t_info *info, t_philo *philos, t_mutex *forks)
{
	int		i;

	i = 0;
	while (i < info->number_of_philosophers)
	{
		philos[i].id = i;
		philos[i].forks = forks;
		philos[i].info = info;
		pthread_create(&philos[i].thread, NULL,
				(void *(*)(void *))philosopher, &philos[i]);
		i++;
	}
	while (i >= 0)
	{
		pthread_join(philos[i].thread, NULL);
		i--;
	}
}

int		main(int ac, char **av)
{
	static t_info	info;
	static t_philo	philos[100];
	static t_mutex	forks[100];

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
	launch(&info, philos, forks); 
}
