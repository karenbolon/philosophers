/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 12:07:21 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/24 16:57:58 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//mine ish
int	initialise_philosophers(t_general *general)
{
	int		i;

	i = 0;
	general->philosophers = malloc(sizeof(t_philo) * \
		general->number_of_philosophers);
	if (!general->philosophers)
		return (FAILURE);
	while (i < general->number_of_philosophers)
	{
		general->philosophers[i].id = i + 1;
		general->philosophers[i].left_fork = &general->fork_mutex[i];
		general->philosophers[i].right_fork = &general->fork_mutex[(i + 1) % 
			general->number_of_philosophers];
		general->philosophers[i].general = general;
		general->philosophers[i].time_to_die = general->time_to_die;
		general->philosophers[i].time_to_eat = general->time_to_eat;
		general->philosophers[i].time_to_sleep = general->time_to_sleep;
		general->philosophers[i].last_meal = 0;
		general->philosophers[i].is_eating = 0;
		general->philosophers[i].number_of_meals = 0;
		i++;
	}
	return (SUCCESS);
}

int		structs(t_general *general, char **list)
{
	general->number_of_philosophers = ft_atoi(list[0]);
	general->time_to_die = ft_atoi(list[1]);
	general->time_to_eat = ft_atoi(list[2]);
	general->time_to_sleep = ft_atoi(list[3]);
	if (list[4])
		general->number_of_meals  = (ft_atoi(list[4]));
	else
		general->number_of_meals = -1;
	general->philosopher_dead = 0;
	if (mutex(general) == FAILURE)
		return (FAILURE);
	if (initialise_philosophers(general) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	mutex(t_general *general)
{
	int	i;

	i = -1;
	general->fork_mutex = malloc(sizeof(pthread_mutex_t) * \
		general->number_of_philosophers);
	if (!general->fork_mutex)
		return (FAILURE);
	while (++i < general->number_of_philosophers)
		pthread_mutex_init(&general->fork_mutex[i], NULL);
	pthread_mutex_init(&general->mutex, NULL);
	return (SUCCESS);
}

void	unlock_philosophers_mutex(t_general *philosophers)
{
	pthread_mutex_unlock(philosophers->left_fork);
	pthread_mutex_unlock(philosophers->right_fork);
}
