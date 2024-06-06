/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:40:31 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/06 22:52:28 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//fcn gets the time, fills a time struct: tv_sec->seconds, tv_usec->milliseconds
//we need to convert to add together
//gettimeofday returns 0 if successful
/*int	ft_timestamp(void)
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
//message in red
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

void	ft_exit(t_table *table, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->table_lock);
	pthread_mutex_destroy(&table->dead_lock);
	while (i < table->philos[0].total_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}*/

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
		usleep(200);//was 500
	return (0);
}

/*void	print_message(char *str, t_philo *philo, int id)
{
	int	time;

	pthread_mutex_lock(philo->write_lock);
	time = get_current_time() - philo->start_time;
	if (!dead_loop(philo))
		printf("%d %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->write_lock);
}*/

//print message checks if philo is dead and if so, prints
//message in red
//mine
void	print_message(char *s, t_philo *philo, int id)
{
	int	time;

	pthread_mutex_lock(philo->write_lock);
	time = ft_timestamp() - philo->start_time;
	if (!dead_loop(philo))
//		printf("%i %i %s\n", time, philo->id, s);
		printf("%i %i %s\n", time, id, s);
	pthread_mutex_unlock(philo->write_lock);
}
//mine
int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

/*void	ft_exit(char *s, t_program *program, pthread_mutex_t * forks)
{
	int	i;

	i = 0;
	if (s)
	{
		while (s)
			write(1, s++, 1);
		write (1, "\n", 1);
		pthread_mutex_destroy(&program->write_lock);
		pthread_mutex_destroy(&program->meal_lock);
		pthread_mutex_destroy(&program->dead_lock);
		while (i < program->philos[0].num_of_philos)
		{
			pthread_mutex_destroy(&forks[i]);
			i++;
		}
	}
}*/

// Destroys all the mutexes

void	destory_all(char *str, t_program *program, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
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

/*int	ft_usleep(int milliseconds)
{
	int	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);//500
	return (0);
}*/

// Gets the current time in milliseconds

/*int	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}*/
