/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophical_activities.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:52:01 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/25 18:28:23 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//checked
void	philosophers_is_eating(t_philo *philosophers)
{
	print_message(philosophers, "is eating");
	pthread_mutex_lock(&philosophers->general->mutex);
	philosophers->last_meal = ft_timestamp() - \
		philosophers->general->starting_time;
	philosophers->time_to_die = philosophers->last_meal + \
		philosophers->general->time_to_die;
	pthread_mutex_unlock(&philosophers->general->mutex);
	ft_sleep(philosophers->general->time_to_eat, philosophers);
	pthread_mutex_lock(&philosophers->general->mutex);
	if (philosophers->number_of_meals != -1)
	{
		philosophers->number_of_meals++;
//		if (philosophers->number_of_meals >= philosophers->general->number_of_meals)
//			philosophers->general->is_full = 1;
	}
	pthread_mutex_unlock(&philosophers->general->mutex);
	pthread_mutex_unlock(philosophers->left_fork);
	pthread_mutex_unlock(philosophers->right_fork);
}
//checked
void	philosophers_is_sleeping(t_philo *philosophers)
{
	print_message(philosophers, "is sleeping");
	ft_sleep(philosophers->general->time_to_sleep, philosophers);
}
//checked
void	philosophers_is_thinking(t_philo *philosophers)
{
	print_message(philosophers, "is thinking");
}
//checked
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

//checked
/*int	philosopher_takes_forks(t_philo *philosophers)
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
}*/

int philosopher_takes_forks(t_philo *philosophers)
{
    if (philosophers->id % 2 == 0)
    {
        pthread_mutex_lock(philosophers->right_fork);
        if (!philosopher_is_dead(philosophers))
            print_message(philosophers, "has taken a fork");
        pthread_mutex_lock(philosophers->left_fork);
        if (!philosopher_is_dead(philosophers))
            print_message(philosophers, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philosophers->left_fork);
        if (!philosopher_is_dead(philosophers))
            print_message(philosophers, "has taken a fork");
		if (philosophers->general->number_of_philosophers == 1)
		{
			philosophers->general->philosopher_dead = 1;
			philosopher_is_dead(philosophers);
		}
        pthread_mutex_lock(philosophers->right_fork);
        if (!philosopher_is_dead(philosophers))
            print_message(philosophers, "has taken a fork");
    }
    return 0;
}
