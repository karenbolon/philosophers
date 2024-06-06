/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 12:07:21 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/06 19:05:51 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_input(t_philo *philos, char **list)
{
	philos->num_of_philos = ft_atoi(list[0]);
	philos->time_to_die = ft_atoi(list[1]);
	philos->time_to_eat = ft_atoi(list[2]);
	philos->time_to_sleep = ft_atoi(list[3]);
	if (list[4])
		philos->num_times_to_eat = (ft_atoi(list[4]));
	else
		philos->num_times_to_eat = -1;
}

//struct initialises philosopher structs and fills variables
void	init_philos(t_philo *philos, t_program *program, \
			pthread_mutex_t *forks, char **list)
{
	int		i;

	i = 0;
	while (i < ft_atoi(list[0]))
	{
		philos[i].id = i + 1;
		philos[i].eating = 0;
		philos[i].meals_eaten = 0;
		init_input(&philos[i], list);
		philos[i].start_time = ft_timestamp();
		philos[i].last_meal = ft_timestamp();
		philos[i].write_lock = &program->write_lock;
		philos[i].dead_lock = &program->dead_lock;
		philos[i].meal_lock = &program->meal_lock;
		philos[i].dead = &program->dead_flag;
		philos[i].l_fork = &forks[i];
		if (i == 0)
			philos[i].r_fork = &forks[philos[i].num_of_philos - 1];
		else
			philos[i].r_fork = &forks[i - 1];
//		init_input(&philos[i], list);
		i++;
	}
}

/*void	init_forks(pthread_mutex_t *forks, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}*/

void	init_program(t_program *program, t_philo *philos)
{
	program->dead_flag = 0;
	program->philos = philos;
	pthread_mutex_init(&program->write_lock, NULL);
	pthread_mutex_init(&program->dead_lock, NULL);
	pthread_mutex_init(&program->meal_lock, NULL);
}

int	thread_create(t_program *program, pthread_mutex_t *forks)
{
	pthread_t	table;
	int			i;

	if (pthread_create(&table, NULL, &ft_monitor, program->philos) != 0)
		ft_exit(program, forks);
	i = 0;
	while (i < program->philos[0].num_of_philos)
	{
		if (pthread_create(&program->philos[i].thread, NULL, \
			&philo_routine, &program->philos[i]) != 0)
		{
			ft_exit(program, forks);
		}
		i++;
	}
	i = 0;
	if (pthread_join(table, NULL) != 0)
		ft_exit(program, forks);
	while (i < program->philos[0].num_of_philos)
	{
		if (pthread_join(program->philos[i].thread, NULL) != 0)
			ft_exit(program, forks);
		i++;
	}
	return (0);
}

void	init_forks(pthread_mutex_t *forks, int count, int i)
{
	if (i < count)
	{
		pthread_mutex_init(&forks[i], NULL);
		init_forks(forks, count, i + 1);
		i++;
	}
}

