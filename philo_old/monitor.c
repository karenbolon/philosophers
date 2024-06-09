/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:11:21 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/24 17:02:57 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*int	dead_or_alive_check(t_phil *phil)
{
	int			time_of_death;
	int			time;
	const char	*red;

	time = ft_ft_timestamp();
	red = "\033[31m";
	time_of_death = time - (phil->table->start_time);
	if ((time - phil->last_meal_time) > phil->time_to_die)
	{
		pthread_mutex_lock(phil->table->meal_mutex);
		if (!phil->table->is_dead)
		{
			phil->table->is_dead = 1;
			printf("%s%d philsopher %d died\n", red, time_of_death, phil->phil_nbr);
		}
		pthread_mutex_unlock(phil->table->meal_mutex);
		return (1);
	}
	return (0);
}

void	*phil_procedures(void *arg)
{
	t_phil	*phil;

	phil = (t_phil *)arg;
	while (1)
	{
		pthread_mutex_lock(phil->table->meal_mutex);
		if (phil->table->is_dead)
		{
			pthread_mutex_unlock(phil->table->meal_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(phil->table->meal_mutex);
		if (dead_or_alive_check(phil))
			return (NULL);
		if (grab_forks(phil))
			return (NULL);
		if (dead_or_alive_check(phil))
			return (NULL);
		phil_eat(phil);
		if (dead_or_alive_check(phil))
			return (NULL);
		phil_sleep(phil);
		if (dead_or_alive_check(phil))
			return (NULL);
		phil_think(phil);
	}
	return (NULL);
}

void	create_and_join_threads(t_table	*table)
{
	int	i;

	i = 0;
	while (i < table->total_phils)
	{
		pthread_create(&table->phil[i].thread_id, NULL, &phil_procedures, \
			(void *)&table->phil[i]);
		i++;
	}
	i = 0;
	while (i < table->total_phils)
	{
		pthread_join(table->phil[i].thread_id, NULL);
		i++;
	}
}

void	fork_message(t_phil *phil)
{
	printf("%d philsopher %d has taken a fork\n", ft_ft_timestamp() - \
		phil->table->start_time, phil->phil_nbr);
}

int	grab_forks(t_phil *phil)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if(phil->phil_nbr % 2 == 0)
	{
		first_fork = phil->r_fork;
		second_fork = phil->l_fork;
	}
	else
	{
		first_fork = phil->l_fork;
		second_fork = phil->r_fork;
	}
	pthread_mutex_lock(first_fork);
	if (dead_or_alive_check(phil))
	{
		pthread_mutex_unlock(first_fork);
		return (1);
	}
	fork_message(phil);
	pthread_mutex_lock(second_fork);
	if (dead_or_alive_check(phil))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (1);
	}
	fork_message(phil);
	return (0);
}*/
///////////////////////////////////////////

/*void	*ft_est(void *phil)
{
	t_philo	*philo;

	philo = (t_philo *)phil;
	if (philo->id % 2 != 0)
		usleep(10000);
	while (!(philo->table->is_dead))
	{
		p_eat(philo);
		if (philo->table->is_dead)
			break ;
		if (philo->table->max_eat != -1 && philo->table->all_ate)
			break ;
		if (philo->table->max_eat != -1
			&& philo->eat_count >= philo->table->max_eat)
			break ;
		print_message(philo, "is sleeping");
		usleep(philo->table->time_sleep * 1000);
		print_message(philo, "is thinking");
	}
	return (NULL);
}


void	p_eat(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	if (philo->id %2 == 0)
	{
		pthread_mutex_lock(&(table->forks[philo->left_fork]));
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&(table->forks[philo->right_fork]));
	}
	else
	{
		pthread_mutex_lock(&(table->forks[philo->right_fork]));
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(&(table->forks[philo->left_fork]));
	}
	print_message(philo, "has taken a fork");
	
	pthread_mutex_lock(&(table->meal_check));
	print_message(philo, "is eating");
	philo->last_ate = ft_timestamp();
	pthread_mutex_unlock(&(table->meal_check));
	usleep(table->time_eat * 1000);
	(philo->eat_count)++;
	if (philo->id %2 == 0)
	{	
		pthread_mutex_unlock(&(table->forks[philo->right_fork]));
		pthread_mutex_unlock(&(table->forks[philo->left_fork]));
	}
	else
	{	
		pthread_mutex_unlock(&(table->forks[philo->left_fork]));
		pthread_mutex_unlock(&(table->forks[philo->right_fork]));
	}
}


void	eat_check(t_table *table, t_philo **phil)
{
	int	i;

	i = 0;
	while (table->max_eat != -1 && i < table->philo_count \
		&& phil[i]->eat_count >= table->max_eat)
		i++;
	if (i == table->philo_count)
		table->all_ate = 1;
}

void	check_is_dead(t_table *table, t_philo **philos)
{
	int	i;
	int	j;
	
	while(!(table->all_ate))
	{
		i = 0;
		while (i < table->philo_count && !(table->is_dead))
		{
			pthread_mutex_lock(&(table->meal_check));
			if (time_diff(philos[i]->last_ate, ft_timestamp()) > table->time_death)
			{
				print_message(philos[i], "died");
				table->is_dead = 1;
				j = 0;
				while (j < table->philo_count)
				{
					pthread_mutex_unlock(&(table->forks[j]));
					j++;
				}
			}
			pthread_mutex_unlock(&(table->meal_check));
//			usleep(500);
			i++;
		}
		if (table->is_dead)
			break ;
		eat_check(table, philos);
	}
}*/
