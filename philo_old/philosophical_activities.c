/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophical_activities.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:52:01 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/24 17:10:50 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philosophers_is_eating(t_philo *philosophers)
{
	print_message(philosophers, "is eating");
	pthread_mutex_lock(&philosophers->general->mutex);
	philosophers->last_meal = ft_timestamp() - 
		philosophers->general->starting_time;
	philosophers->time_to_die = philosophers->last_meal + 
		philosophers->general->time_to_die;
	pthread_mutex_unlock(&philosophers->general->mutex);
	ft_sleep(philosophers->general->time_to_eat, philosophers);
	pthread_mutex_lock(&philosophers->general->mutex);
	if (philosophers->number_of_meals != -1)
		philosophers->number_of_meals++;
	pthread_mutex_unlock(&philosophers->general->mutex);
	pthread_mutex_unlock(philosophers->left_fork);
	pthread_mutex_unlock(philosophers->right_fork);
}

void	philosophers_is_sleeping(t_philo *philosophers)
{
	print_message(philosophers, "is sleeping");
	ft_sleep(philosophers->general->time_to_sleep, philosophers);
}

void	philosophers_is_thinking(t_philo *philosophers)
{
	print_message(philosophers, "is thinking");
}

int	philosopher_is_dead(t_philo *philosophers)
{
	pthread_mutex_lock(&philosophers->general->mutex);
	if (philosophers->general->philosopher_dead == 1)
	{
		pthread_mutex_unlock(&philosophers->general->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philosophers->general->mutex);
	return (0);
}

int	philosopher_takes_forks(t_philo *philosophers)
{
	if (philosophers->id %2 == 0)
		pthread_mutex_lock(philosophers->right_fork);
	else
		pthread_mutex_lock(philosophers->left_fork);
	if (!philosopher_is_dead(philosophers))
		print_message(philosophers, "has taken a fork");
	if (philosophers->id %2 == 0)
	{
		if (pthread_mutex_lock(philosophers->left_fork) != 0)
		{
			pthread_mutex_unlock(philosophers->right_fork);
			return (1);
		}
		if (!philosopher_is_dead(philosophers))
			print_message(philosophers, "has taken a fork");
	}
	else
	{
		if (pthread_mutex_lock(philosophers->right_fork) != 0)
		{
			pthread_mutex_unlock(philosophers->left_fork);
			return (1);
		}
		if (!philosopher_is_dead(philosophers))
			print_message(philosophers, "has taken a fork");
	}
	return (0);
}

int	check_last_meal(t_philo *philo)
{
	if (ft_timestamp() - philo->general->starting_time > philo->time_to_die)
	{
		philo->general->philosopher_dead = 1;
		printf("%s %d %d died\n", RED, ft_timestamp() - 
			philo->general->starting_time, philo->id);
		return (0);
	}
	else
		return (1);
}

int	check_if_one_is_dead(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->number_of_philosophers)
	{
		if (check_last_meal(&general->philosophers[i]) == ERROR)
			return (ERROR);
		i++;
	}
	return (1);
}

void	free_philosophers(t_general *philosophers)
{
	free(philosophers->philosophers);
	free(philosophers->fork_mutex);
}

void	*begin_monitoring(void *arg)
{
	t_general	*data;

	data = (t_general *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		if (check_if_one_is_dead(data) == ERROR)
		{
			usleep(100);
			pthread_mutex_unlock(&data->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&data->mutex);
		usleep(200);
	}
}

void	*philosophers_routine(void *arguments)
{
	t_philo	*philosophers;

	philosophers = (t_philo *)arguments;
	while (philosophers->general->philosopher_dead == 0)
	{
		if (philosopher_is_dead(philosophers))
			return (NULL);
		philosopher_takes_forks(philosophers);
		if (philosopher_is_dead(philosophers))
		{
			pthread_mutex_unlock(philosophers->left_fork);
			pthread_mutex_unlock(philosophers->right_fork);
			return (NULL);
		}
		philosophers_is_eating(philosophers);
		if (philosopher_is_dead(philosophers))
			return (NULL);
		philosophers_is_sleeping(philosophers);
		if (philosopher_is_dead(philosophers))
			return (NULL);
		philosophers_is_thinking(philosophers);
	}
	return (NULL);
}

void	join_threads(t_general *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		i++;
	}
}

void	begin_philosophers_routine(t_general *data)
{
	int	i;

	i = 0;
	data->starting_time = ft_timestamp();
	while (i < data->number_of_philosophers)
	{
		pthread_create(&data->philosophers[i].thread_id, NULL,
			&philosophers_routine, (void *)&data->philosophers[i]);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		i++;
	}
}

void	ft_sleep(int time, t_philo *philosophers)
{
	int	start;

	start = ft_timestamp();
	while ((ft_timestamp() - start) < time && \
		(philosopher_is_dead(philosophers) == FALSE))
		usleep(50);
}