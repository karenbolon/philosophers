/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:51:15 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/07 14:24:10 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	char			**list;
	t_general		restuarant;
	t_philo			philos[MAX_PHIL];
	pthread_mutex_t	forks[MAX_PHIL];

	list = NULL;
	if (ac < 5 || ac > 6)
	{
		printf("Please enter the correct parameters\n");
		return (1);
	}
	list = av + 1;
	if (ft_check_args(list) == 1)
		return (1);
	init_general(&restuarant, philos, forks, ft_atoi(list[0]));
	init_philos(philos, &restuarant, forks, list);
	thread_create(&restuarant, forks);
	ft_exit(&restuarant, forks);
	return (0);
}
