/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophical_activities.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:52:01 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/10 11:49:59 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*picking up forks by id numbers (even vs odd) leads to deadlock 
(if all pick up 1 fork and wait for 2nd) or starvation situation.
Philos eat based on id's & always picks up the lowest # fork
Even id's sleep immediately, which staggers the eating attempts
*/
void	philo_is_eating(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2 == 0)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	print_message("has taken a fork", philo);
	if (philo->num_of_philos == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(first_fork);
		return ;
	}
	ft_eat_more(philo, first_fork, second_fork);
}

void	ft_eat_more(t_philo *philo, pthread_mutex_t	*first_fork, \
			pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(second_fork);
	print_message("has taken a fork", philo);
	print_message("is eating", philo);
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = ft_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_lock);
	ft_usleep(philo->time_to_eat);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(philo->time_to_sleep);
	else if (philo->id % philo->num_of_philos != 0)
		ft_usleep(1);
	while (!check_for_dead_philos(philo))
	{
		philo_is_eating(philo);
		print_message("is sleeping", philo);
		ft_usleep(philo->time_to_sleep);
		print_message("is thinking", philo);
	}
	return (arg);
}
