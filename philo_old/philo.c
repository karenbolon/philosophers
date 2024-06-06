/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:51:15 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/05 08:00:57 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
//	int				i;
	char			**list;
	t_general		general;

	list = NULL;
//	i = 0;
	if (ac < 5 || ac > 6)
	{
		printf("Please enter the correct parameters\n");
		return (1);
	}
	list = av + 1;
	if (ft_check_args(list) == 1)
		return (1);
	if (structs(&general, list))
	{
		begin_philosophers_routine(&general);
		begin_monitoring(&general);
		join_threads(&general);
		free_philosophers(&general);
	}
	return (0);
}
