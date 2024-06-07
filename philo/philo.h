/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 09:08:02 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/07 14:06:15 by kbolon           ###   ########.fr       */
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

# ifndef MAX_PHIL
#  define MAX_PHIL 200
# endif

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	int					last_meal;
	int					time_to_die;
	int					time_to_sleep;
	int					time_to_eat;
	int					start_time;
	int					num_of_philos;
	int					num_times_to_eat;
	int					*dead;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*write_lock;
	pthread_mutex_t		*dead_lock;
	pthread_mutex_t		*meal_lock;
}	t_philo;

typedef struct s_general
{
	int					dead_flag;
	pthread_mutex_t		dead_lock;
	pthread_mutex_t		meal_lock;
	pthread_mutex_t		write_lock;
	t_philo				*philos;
}	t_general;

//arg_checker.c
int		check_input(char **list);
int		check_for_int(long nbr);
int		ft_check_args(char **list);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);

//init.c
void	init_input(t_philo *philos, char **list);
void	init_philos(t_philo *philos, t_general *restuarant, 
			pthread_mutex_t *forks, char **list);
void	init_general(t_general *restuarant, t_philo *philos, \
			pthread_mutex_t *forks, int count);
int		thread_create(t_general *restuarant, pthread_mutex_t *forks);

//monitor.c
int		check_if_dead(t_philo *philos);
int		check_if_all_full(t_philo *philos);
void	*monitor(void *arg);
int		check_for_dead_philos(t_philo *philo);

//philo_utils.c
int		ft_timestamp(void);
int		ft_usleep(int time);
void	print_message(char *s, t_philo *philo);
void	ft_exit(t_general *restuarant, pthread_mutex_t *forks);

//philosophical_activities.c
void	philo_is_eating(t_philo *philo);
void	ft_eat_more(t_philo *philo, pthread_mutex_t	*first_fork, 
			pthread_mutex_t *second_fork);
void	*philo_routine(void *arg);

#endif
