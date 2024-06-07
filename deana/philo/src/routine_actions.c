/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 08:48:15 by druina            #+#    #+#             */
/*   Updated: 2024/06/06 16:57:37 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Think routine funtion

void	think(t_philo *philo)
{
	print_message("is thinking", philo, philo->id);
}

// Dream routine funtion

void	dream(t_philo *philo)
{
	print_message("is sleeping", philo, philo->id);
	ft_usleep(philo->time_to_sleep);
}

// Eat routine funtion

//mine
void eat(t_philo *philo)
{
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    if (philo->id % 2 == 0) {
        first_fork = philo->r_fork;
        second_fork = philo->l_fork;
    } else {
        first_fork = philo->l_fork;
        second_fork = philo->r_fork;
    }

    pthread_mutex_lock(first_fork);
    print_message("has taken a fork", philo, philo->id);
    if (philo->num_of_philos == 1)
    {
        ft_usleep(philo->time_to_die);
        pthread_mutex_unlock(first_fork);
        return;
    }
    pthread_mutex_lock(second_fork);
    print_message("has taken a fork", philo, philo->id);
//    philo->eating = 1;
    print_message("is eating", philo, philo->id);
    pthread_mutex_lock(philo->meal_lock);
    philo->last_meal = ft_timestamp();
    philo->meals_eaten++;
    pthread_mutex_unlock(philo->meal_lock);
    ft_usleep(philo->time_to_eat);
//    philo->eating = 0;
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}