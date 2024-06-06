/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:40:31 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/05 20:50:26 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
fcn gets the time, fills a time struct: tv_sec->seconds, tv_usec->milliseconds
we need to convert to add together
gettimeofday returns 0 if successful
*/
int	ft_timestamp(void)
{
	int					time;
	struct timeval		time_var;

	time = 0;
	if (!gettimeofday(&time_var, NULL))
		time = time_var.tv_sec * 1000 + time_var.tv_usec / 1000;
	return (time);
}

/*
print message checks if philo is dead
*/
void	print_message(t_philo *philo, char *s)
{
	if (!(philosopher_is_dead(philo)))
	{
		pthread_mutex_lock(&philo->table->table_mutex);
		printf("%i %i %s\n", ft_timestamp() - \
			philo->table->start_time, philo->id, s);
		pthread_mutex_unlock(&(philo->table->table_mutex));
	}
	if (philosopher_is_dead(philo))
	{
		pthread_mutex_lock(&philo->table->table_mutex);
		printf("%i %i %s\n", ft_timestamp() + 1 - \
			philo->table->start_time, philo->id, s);
		pthread_mutex_unlock(&(philo->table->table_mutex));
	}
}

void	freedom_function(t_table *table)
{
	if (table->philos)
		free(table->philos);
	if (table->fork_mutex)
		free(table->fork_mutex);
}
