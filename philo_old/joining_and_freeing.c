/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining_and_freeing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 06:43:38 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/24 17:02:29 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
loops through each philo array, joining each thread
and freeing the corresponding object
*/
/*void	joining_thread_and_destroying(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		pthread_join(table->philos[i]->thread_id, NULL);
		free(table->philos[i]);
		i++;
	}
	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_destroy(&(table->forks[i]));
		i++;
	}
	pthread_mutex_destroy(&(table->writing));
	pthread_mutex_destroy(&(table->meal_check));
	free(table->philos);
	free(table);
}*/
