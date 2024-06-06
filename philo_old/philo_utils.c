/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:40:31 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/24 17:19:00 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_timestamp(void)
{
	struct timeval		time_var;

	gettimeofday(&time_var, NULL);
	return ((time_var.tv_sec * 1000) + (time_var.tv_usec/1000));
}


void	print_message(t_philo *philosopher, char *msg)
{
	if (!(philosopher_is_dead(philosopher)))
	{
		pthread_mutex_lock(&philosopher->general->mutex);
		printf("%i %i %s\n", ft_timestamp() - \
			philosopher->general->starting_time, philosopher->id, msg);
		pthread_mutex_unlock(&(philosopher->general->mutex));
	}
	if (philosopher_is_dead(philosopher))
	{
		printf("%s %i %i %s\n", RED, ft_timestamp() - \
			philosopher->general->starting_time, philosopher->id, msg);
		pthread_mutex_unlock(&(philosopher->general->mutex));
	}
}
