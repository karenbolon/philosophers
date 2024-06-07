/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 12:07:21 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/07 14:05:27 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_input(t_philo *philos, char **list)
{
	philos->time_to_die = ft_atoi(list[1]);
	philos->time_to_eat = ft_atoi(list[2]);
	philos->time_to_sleep = ft_atoi(list[3]);
	philos->num_of_philos = ft_atoi(list[0]);
	if (list[4])
		philos->num_times_to_eat = (ft_atoi(list[4]));
	else
		philos->num_times_to_eat = -1;
}

//struct initialises philosopher structs and fills variables
//if philo id = 0, the right fork is total number of philos - 1
void	init_philos(t_philo *philos, t_general *restuarant, \
			pthread_mutex_t *forks, char **list)
{
	int		i;

	i = 0;
	while (i < ft_atoi(list[0]))
	{
		init_input(&philos[i], list);
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].start_time = ft_timestamp();
		philos[i].last_meal = ft_timestamp();
		philos[i].write_lock = &restuarant->write_lock;
		philos[i].dead_lock = &restuarant->dead_lock;
		philos[i].meal_lock = &restuarant->meal_lock;
		philos[i].dead = &restuarant->dead_flag;
		philos[i].left_fork = &forks[i];
		if (i == 0)
			philos[i].right_fork = &forks[philos[i].num_of_philos - 1];
		else
			philos[i].right_fork = &forks[i - 1];
		i++;
	}
}

void	init_general(t_general *restuarant, t_philo *philos, \
			pthread_mutex_t *forks, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	restuarant->dead_flag = 0;
	restuarant->philos = philos;
	pthread_mutex_init(&restuarant->write_lock, NULL);
	pthread_mutex_init(&restuarant->dead_lock, NULL);
	pthread_mutex_init(&restuarant->meal_lock, NULL);
}

int	thread_create(t_general *restaurant, pthread_mutex_t *forks)
{
	pthread_t	waiter;
	int			i;

	if (pthread_create(&waiter, NULL, &monitor, restaurant->philos) != 0)
		ft_exit(restaurant, forks);
	i = 0;
	while (i < restaurant->philos[0].num_of_philos)
	{
		if (pthread_create(&restaurant->philos[i].thread, NULL, \
			&philo_routine, &restaurant->philos[i]) != 0)
			ft_exit(restaurant, forks);
		i++;
	}
	i = 0;
	if (pthread_join(waiter, NULL) != 0)
		ft_exit(restaurant, forks);
	while (i < restaurant->philos[0].num_of_philos)
	{
		if (pthread_join(restaurant->philos[i].thread, NULL) != 0)
			ft_exit(restaurant, forks);
		i++;
	}
	return (0);
}
