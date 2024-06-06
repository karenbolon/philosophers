/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 09:17:55 by druina            #+#    #+#             */
/*   Updated: 2024/06/06 15:53:42 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Checks the len of the string

int	ft_strlen(char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

// Destroys all the mutexes

void	destory_all(char *str, t_program *program, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		while (*str)
		{
//		write(2, str, ft_strlen(str));
//		write(2, "\n", 1);
			write(1, str++, 1);
		}
		write (1, "\n", 1);
	}
	pthread_mutex_destroy(&program->write_lock);
	pthread_mutex_destroy(&program->meal_lock);
	pthread_mutex_destroy(&program->dead_lock);
	while (i < program->philos[0].num_of_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

// Improved version of sleep function

int	ft_usleep(int milliseconds)
{
	int	start;

	start = ft_timestamp();
	while ((ft_timestamp() - start) < milliseconds)
		usleep(500);
	return (0);
}

//mine
int	ft_timestamp(void)
{
	int					time;
	struct timeval		time_var;

	time = 0;
	if (!gettimeofday(&time_var, NULL))
		time = time_var.tv_sec * 1000 + time_var.tv_usec / 1000;
	return (time);
}
