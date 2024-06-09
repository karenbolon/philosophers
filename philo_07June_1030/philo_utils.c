/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:40:31 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/07 10:11:31 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//fcn gets the time, fills a time struct: tv_sec->seconds, tv_usec->milliseconds
//we need to convert to add together
//gettimeofday returns 0 if successful
int	ft_timestamp(void)
{
	int					time;
	struct timeval		time_var;

	time = 0;
	if (!gettimeofday(&time_var, NULL))
		time = time_var.tv_sec * 1000 + time_var.tv_usec / 1000;
	return (time);
}

int	ft_usleep(int time)
{
	int	start;

	start = ft_timestamp();
	while ((ft_timestamp() - start) < time)
		usleep(500);
	return (0);
}

//print message checks if philo is dead and if so, prints
void	print_message(char *s, t_philo *philo)
{
	int	time;

	pthread_mutex_lock(philo->write_lock);
	time = ft_timestamp() - philo->start_time;
	if (!dead_loop(philo))
		printf("%i %i %s\n", time, philo->id, s);
	pthread_mutex_unlock(philo->write_lock);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_exit(t_general *table, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->meal_lock);
	pthread_mutex_destroy(&table->dead_lock);
	while (i < table->philos[0].num_of_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}
