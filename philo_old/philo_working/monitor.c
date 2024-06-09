/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:11:21 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/25 18:29:35 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//checked
int	check_last_meal(t_philo *philo)
{
	if (ft_timestamp() - philo->general->starting_time > philo->time_to_die)
	{
		philo->general->philosopher_dead = 1;
		printf("%s %d %d died\n", RED, ft_timestamp() - 
			philo->general->starting_time, philo->id);
		return (0);
	}
	else
		return (1);
}
//checked
int	check_if_one_is_dead(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->number_of_philosophers)
	{
		if (check_last_meal(&general->philosophers[i]) == ERROR)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	check_all_philosophers_done(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->number_of_philosophers)
	{
		if (general->philosophers[i].number_of_meals != -1 && \
			general->philosophers[i].number_of_meals < general->number_of_meals)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

//checked
void	free_philosophers(t_general *philosophers)
{
	free(philosophers->philosophers);
	free(philosophers->fork_mutex);
}
//checked
void	*begin_monitoring(void *arg)
{
	t_general	*data;

	data = (t_general *)arg;

	while (data->is_full == 0 && data->philosopher_dead == 0)
	{
		pthread_mutex_lock(&data->mutex);
		if (check_if_one_is_dead(data) == ERROR)
		{
			usleep(100);
			pthread_mutex_unlock(&data->mutex);
			return (NULL);
		}
		if (data->number_of_meals != -1 && check_all_philosophers_done(data) == TRUE)
		{
			data->is_full = 1;
			pthread_mutex_unlock(&data->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&data->mutex);
		usleep(200);
	}
	return (NULL);
}
//checked
void	*philosophers_routine(void *arguments)
{
	t_philo	*philosophers;

	philosophers = (t_philo *)arguments;
	while (philosophers->general->philosopher_dead == 0 && philosophers->general->is_full == 0)
	{
		if (philosopher_is_dead(philosophers))
			return (0);
		if (philosophers->general->number_of_philosophers == 1)
		{
			print_message(philosophers, "has taken a fork");
			ft_sleep(philosophers->general->time_to_die + 1, philosophers);
			philosophers->general->philosopher_dead = 1;
			print_message(philosophers, "died");
			return (0);
		}
		philosopher_takes_forks(philosophers);
		if (philosopher_is_dead(philosophers))
		{
			pthread_mutex_unlock(philosophers->left_fork);
			pthread_mutex_unlock(philosophers->right_fork);
			return (0);
		}
		philosophers_is_eating(philosophers);
		if (philosopher_is_dead(philosophers))
			return (0);
		philosophers_is_sleeping(philosophers);
		if (philosopher_is_dead(philosophers))
			return (0);
		philosophers_is_thinking(philosophers);
	}
	return (0);
}

void	join_threads(t_general *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		i++;
	}
}
//checked
void	begin_philosophers_routine(t_general *data)
{
	int	i;

	i = 0;
	data->starting_time = ft_timestamp();
	while (i < data->number_of_philosophers)
	{
		pthread_create(&data->philosophers[i].thread_id, NULL,
			&philosophers_routine, (void *)&data->philosophers[i]);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		i++;
	}
}
//checked
void	ft_sleep(int time, t_philo *philosophers)
{
	int	start;

	start = ft_timestamp();
	while ((ft_timestamp() - start) < time && \
		(philosopher_is_dead(philosophers) == FALSE))
		usleep(50);
}
