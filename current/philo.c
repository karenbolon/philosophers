/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:51:15 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/05 18:18:28 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void cleanup_resources(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		pthread_mutex_destroy(&table->fork_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&table->table_mutex);
	free(table->philos);
	free(table->fork_mutex);
}

int	main(int ac, char **av)
{
	char		**list;
	t_table		table;
	pthread_t	monitor_thread;

	list = NULL;
	if (ac < 5 || ac > 6)
	{
		printf("Please enter the correct parameters\n");
		return (1);
	}
	list = av + 1;
	if (ft_check_args(list) == 1)
		return (1);
	if (init_structs(&table, list) == 0)
		return (1);
	pthread_create(&monitor_thread, NULL, &begin_monitoring, (void *)&table);
	begin_routine(&table);
	pthread_join(monitor_thread, NULL);
	join_threads(&table);
	cleanup_resources(&table);
	return (0);
}
