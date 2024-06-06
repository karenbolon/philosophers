/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 09:08:02 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/06 10:46:22 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <limits.h>

# define RED "\033[1;31m"

# ifndef MAX_PHIL
#  define MAX_PHIL 200
# endif

typedef struct s_philo
{
	int					id;
	int					time_since_last_meal;
	int					time_to_die;
	int					time_to_sleep;
	int					time_to_eat;
	int					meals_eaten;
	pthread_t			thread_id;
	struct s_table		*table;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
}	t_philo;

typedef struct s_table
{
	int					number_of_philosophers;
	int					total_meals_to_eat;
	int					start_time;
	int					is_dead;
	int					is_full;
	pthread_mutex_t		*fork_mutex;
	pthread_mutex_t		table_mutex;
	t_philo				*philos;
}	t_table;

//arg_checker.c
int		check_input(char **list);
int		check_for_int(long nbr);
int		ft_check_args(char **list);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);

//init.c
int		init_philosophers(t_table *table, char **list);
int		init_structs(t_table *table, char **list);
void	join_threads(t_table *table);
void	begin_routine(t_table *table);
void	one_philo_found(t_table *table);

//philo_utils.c
int		ft_timestamp(void);
void	print_message(t_philo *philo, char *s);
void	freedom_function(t_table *table);

//monitor.c
int		check_for_dead_philos(t_table *table);
int		check_all_philosophers_done(t_table *table);
void	*begin_monitoring(void *arg);
void	*philosophers_routine(void *arguments);
int		check_for_one_philo(t_philo *philo);

//philosophical_activities.c
void	philo_is_eating(t_philo *philo);
void	philo_is_sleeping(int time, t_philo *philo);
int		philosopher_is_dead(t_philo *philo);
int		philosopher_takes_forks(t_philo *philo);
int		odd_philosophers(t_philo *philo);

#endif