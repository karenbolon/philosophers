/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:11:21 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/06 10:08:45 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//checks if a philosopher has died and if so, prints it in red to see easier
/*int	check_for_dead_philos(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		philo = &table->philos[i];
		if (ft_timestamp() - philo->table->start_time > philo->time_to_die)
		{
			pthread_mutex_lock(&table->table_mutex);
			philo->table->is_dead = 1;
			printf("%d %d died\n", ft_timestamp() - \
				philo->table->start_time, philo->id);
			pthread_mutex_unlock(&table->table_mutex);
			return (0);
		}
		i++;
	}
	return (1);
}*/

int check_for_dead_philos(t_table *table)
{
    int i;
    t_philo *philo;

    for (i = 0; i < table->number_of_philosophers; i++)
    {
        philo = &table->philos[i];
        pthread_mutex_lock(&table->table_mutex);
        if (ft_timestamp() - philo->time_since_last_meal > philo->time_to_die)
        {
            table->is_dead = 1;
            printf("%d %d died\n", ft_timestamp() - table->start_time, philo->id);
            pthread_mutex_unlock(&table->table_mutex);
            return (0);
        }
        pthread_mutex_unlock(&table->table_mutex);
    }
    return (1);
}

/*
checks if there is a defined number of meals to eat and iterates through to see
if all philos have reached the target meal number
*/
int	check_all_philosophers_done(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		if (table->philos[i].meals_eaten != -1 && \
			table->philos[i].meals_eaten < table->total_meals_to_eat)
			return (0);
		i++;
	}
	return (1);
}
/*
checks if philospher is dead or all have reached the specified
number of meals have been reached.

Table mutex prevents data races/protects code and ensures shared variables
are properly updated, such as: table->is_dead and table->is_full
*/
void	*begin_monitoring(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		pthread_mutex_lock(&table->table_mutex);
		if (table->is_full == 1 || table->is_dead == 1)
		{
			pthread_mutex_unlock(&table->table_mutex);
			break ;
		}
		pthread_mutex_unlock(&table->table_mutex);
		if (check_for_dead_philos(table) == 0)
		{
//			pthread_mutex_unlock(&table->table_mutex);
			break ;
		}
		pthread_mutex_lock(&table->table_mutex);
		if (table->total_meals_to_eat != -1 && \
			check_all_philosophers_done(table) == 1)
		{
			table->is_full = 1;
			pthread_mutex_unlock(&table->table_mutex);
			break;
		}
		pthread_mutex_unlock(&table->table_mutex);
		usleep(50);//was 1000
	}
	return (NULL);
}


//this functions tells the philosopher what to do and checks
//if they died inbetween actions
//forks must be locked before a philo can "pick up" and use, and unlock when
//"put down".  This prevents philos from using same forks
/*void	*philosophers_routine(void *arguments)
{
	t_philo	*philo;

	philo = (t_philo *)arguments;
	while (1)
	{
		pthread_mutex_lock(&philo->table->table_mutex);
		if (philo->table->is_dead || philo->table->is_full)
		{
			pthread_mutex_unlock(&philo->table->table_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->table->table_mutex);
		if (!check_for_one_philo(philo))
			break ;;
		philosopher_takes_forks(philo);
		pthread_mutex_lock(&philo->table->table_mutex);
		if (philosopher_is_dead(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(&philo->table->table_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->table->table_mutex);
		philo_is_eating(philo);
		pthread_mutex_lock(&philo->table->table_mutex);
		if (philosopher_is_dead(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(&philo->table->table_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->table->table_mutex);
		philo_is_sleeping(philo->time_to_sleep, philo);
		pthread_mutex_lock(&philo->table->table_mutex);
		if (philosopher_is_dead(philo))
		{
			pthread_mutex_unlock(&philo->table->table_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->table->table_mutex);
		print_message(philo, "is thinking");
	}
	return (0);
}*/

void *philosophers_routine(void *arguments)
{
    t_philo *philo = (t_philo *)arguments;

    while (1)
    {
        pthread_mutex_lock(&philo->table->table_mutex);
        if (philo->table->is_dead || philo->table->is_full)
        {
            pthread_mutex_unlock(&philo->table->table_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->table->table_mutex);

        if (!check_for_one_philo(philo))
            break;

        philosopher_takes_forks(philo);

        pthread_mutex_lock(&philo->table->table_mutex);
        if (philosopher_is_dead(philo))
        {
            pthread_mutex_unlock(philo->left_fork);
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(&philo->table->table_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->table->table_mutex);

        philo_is_eating(philo);

        pthread_mutex_lock(&philo->table->table_mutex);
        if (philosopher_is_dead(philo))
        {
            pthread_mutex_unlock(&philo->table->table_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->table->table_mutex);

        philo_is_sleeping(philo->time_to_sleep, philo);

        pthread_mutex_lock(&philo->table->table_mutex);
        if (philosopher_is_dead(philo))
        {
            pthread_mutex_unlock(&philo->table->table_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->table->table_mutex);

        print_message(philo, "is thinking");
    }
    return NULL;
}

/*int	check_for_one_philo(t_philo *philo)
{
	if (philo->table->number_of_philosophers == 1)
	{
		print_message(philo, "has taken a fork");	
		pthread_mutex_lock(&philo->table->table_mutex);
		philo->table->is_dead = 1;
		printf("%d %d died\n", ft_timestamp() + 1 - \
			philo->table->start_time, philo->id);
		pthread_mutex_unlock(&philo->table->table_mutex);
		return (0);
	}
	return (1);
}*/

int check_for_one_philo(t_philo *philo)
{
    if (philo->table->number_of_philosophers == 1)
    {
        print_message(philo, "has taken a fork");
        pthread_mutex_lock(&philo->table->table_mutex);
        philo->table->is_dead = 1;
        printf("%d %d died\n", ft_timestamp() - philo->table->start_time, philo->id);
        pthread_mutex_unlock(&philo->table->table_mutex);
        return (0);
    }
    return (1);
}
