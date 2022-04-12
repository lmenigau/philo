#include "philo.h"

void	take_fork2(t_philo *philo)
{
	long	ts_release;
	long	now;

	pthread_mutex_lock(&philo->right->lock);
	ts_release = philo->right->ts_release;
	if (ts_release > micro_ts())
	{
		pthread_mutex_unlock(&philo->left->lock);
		pthread_mutex_unlock(&philo->right->lock);
		philo->state--;
		sleep_until(philo, ts_release);
	}
	else
	{
		now = micro_ts();
		philo->left->ts_release = now + philo->info->eat_time;
		philo->right->ts_release = now + philo->info->eat_time;
		pthread_mutex_unlock(&philo->left->lock);
		pthread_mutex_unlock(&philo->right->lock);
	}
}

void	take_fork(t_philo *philo)
{
	long	ts_release;

	pthread_mutex_lock(&philo->left->lock);
	ts_release = philo->left->ts_release;
	if (ts_release > micro_ts())
	{
		pthread_mutex_unlock(&philo->left->lock);
		philo->state--;
		sleep_until(philo, ts_release);
	}
	else if (philo->info->maxphil > 1)
		take_fork2(philo);
	else
		sleep_until(philo, philo->ts_dead);
}
