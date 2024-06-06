/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:51:15 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/25 18:22:47 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	char			**list;
	t_general		general;
	pthread_t		monitor_thread;

	list = NULL;
	if (ac < 5 || ac > 6)
	{
		printf("Please enter the correct parameters\n");
		return (1);
	}
	list = av + 1;
	if (ft_check_args(list) == 1)
		return (1);
	if (structs(&general, list) == FAILURE)
		return (1);
	pthread_create(&monitor_thread, NULL, &begin_monitoring, (void *)&general);
	begin_philosophers_routine(&general);
	pthread_join(monitor_thread, NULL);
	join_threads(&general);
	free_philosophers(&general);
	return (0);
}
