/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 12:07:21 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/06 10:07:50 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//struct initialises philosopher structs and fills variables
int	init_philosophers(t_table *table, char **list)
{
	int		i;

	i = 0;
	table->philos = malloc(sizeof(t_philo) * \
		table->number_of_philosophers);
	if (!table->philos)
		return (0);
	while (i < table->number_of_philosophers)
	{
		table->philos[i].id = i + 1;
		table->philos[i].left_fork = &table->fork_mutex[i];
		table->philos[i].right_fork = &table->fork_mutex[(i + 1) % \
			table->number_of_philosophers];
		table->philos[i].table = table;
		table->philos[i].time_to_die = ft_atoi(list[1]);
		table->philos[i].time_to_eat = ft_atoi(list[2]);
		table->philos[i].time_to_sleep = ft_atoi(list[3]);
		table->philos[i].time_since_last_meal = 0;
		table->philos[i].meals_eaten = 0;
		i++;
	}
	return (1);
}


//struct initialises table struct and fills variables
//"sets the table and lays the forks"
/*
int	init_structs(t_table *table, char **list)
{
	int	i;

	i = -1;
	table->number_of_philosophers = ft_atoi(list[0]);
	if (list[4])
		table->total_meals_to_eat = (ft_atoi(list[4]));
	else
		table->total_meals_to_eat = -1;
	table->is_dead = 0;
	table->is_full = 0;
	table->start_time = ft_timestamp();
	table->fork_mutex = malloc(sizeof(pthread_mutex_t) * \
		table->number_of_philosophers);
	if (!table->fork_mutex)
		return (0);
	while (++i < table->number_of_philosophers)
		pthread_mutex_init(&table->fork_mutex[i], NULL);
	pthread_mutex_init(&table->table_mutex, NULL);
	if (init_philosophers(table, list) == 0)
		return (0);
	return (1);
}*/
int init_structs(t_table *table, char **list)
{
    int i;

    table->number_of_philosophers = ft_atoi(list[0]);
    if (list[4])
        table->total_meals_to_eat = (ft_atoi(list[4]));
    else
        table->total_meals_to_eat = -1;
    table->is_dead = 0;
    table->is_full = 0;
    table->start_time = ft_timestamp();

    table->fork_mutex = malloc(sizeof(pthread_mutex_t) * table->number_of_philosophers);
    if (!table->fork_mutex)
        return (0);

    for (i = 0; i < table->number_of_philosophers; i++)
        pthread_mutex_init(&table->fork_mutex[i], NULL);

    pthread_mutex_init(&table->table_mutex, NULL);

    if (init_philosophers(table, list) == 0)
        return (0);

    return (1);
}

void	join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
}

/*void	begin_routine(t_table *table)
{
	int	i;

	i = 0;
//	table->start_time = ft_timestamp();
	while (i < table->number_of_philosophers)
	{
		pthread_create(&table->philos[i].thread_id, NULL,
			&philosophers_routine, (void *)&table->philos[i]);
		i++;
	}
	i = 0;
	while (i < table->number_of_philosophers)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
}*/

void begin_routine(t_table *table)
{
    int i;

    for (i = 0; i < table->number_of_philosophers; i++)
    {
        pthread_create(&table->philos[i].thread_id, NULL,
                       &philosophers_routine, (void *)&table->philos[i]);
    }

    for (i = 0; i < table->number_of_philosophers; i++)
    {
        pthread_join(table->philos[i].thread_id, NULL);
    }
}
