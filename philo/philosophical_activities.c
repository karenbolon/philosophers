/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophical_activities.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:52:01 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/06 11:16:05 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_is_eating(t_philo *philo)
{
	print_message(philo, "is eating");
	pthread_mutex_lock(&philo->table->table_mutex);
	philo->time_since_last_meal = ft_timestamp() - \
		philo->table->start_time;
	philo->time_to_die = philo->time_since_last_meal + \
		philo->time_to_die;
	pthread_mutex_unlock(&philo->table->table_mutex);
	philo_is_sleeping(philo->time_to_eat, philo);
	pthread_mutex_lock(&philo->table->table_mutex);
	if (philo->meals_eaten != -1)
		philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->table_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_is_sleeping(int time, t_philo *philo)
{
	int	start;

	start = ft_timestamp();
	print_message(philo, "is sleeping");
	while ((ft_timestamp() - start) < time && !philosopher_is_dead(philo))
		usleep(100);
}

//checks if individual philo has been marked as dead
//it will return either 0 (alive) or 1 (dead)
int	philosopher_is_dead(t_philo *philo)
{
	int		is_dead;

	pthread_mutex_lock(&philo->table->table_mutex);
	is_dead = philo->table->is_dead;
	pthread_mutex_unlock(&philo->table->table_mutex);
	return (is_dead);
}

int	philosopher_takes_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		if (!philosopher_is_dead(philo))
			print_message(philo, "has taken a fork");
		if (pthread_mutex_lock(philo->left_fork) != 0)
		{
			pthread_mutex_unlock(philo->right_fork);
			return (1);
		}
		if (!philosopher_is_dead(philo))
			print_message(philo, "has taken a fork");
	}
	else
	{
		usleep(200);
		if (odd_philosophers(philo))
			return (1);
	}
	return (0);
}

int	odd_philosophers(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (!philosopher_is_dead(philo))
		print_message(philo, "has taken a fork");
	if (philo->table->number_of_philosophers == 1)
	{
		philo->table->is_dead = 1;
		philosopher_is_dead(philo);
	}
	if (pthread_mutex_lock(philo->right_fork) != 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	if (!philosopher_is_dead(philo))
		print_message(philo, "has taken a fork");
	return (0);
}
