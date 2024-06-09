/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 09:08:02 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/05 08:01:49 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define TRUE		1
# define SUCCESS	1
# define FALSE		0
# define ERROR		0
# define FAILURE	0

# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <limits.h>

# define RED "\033[1;31m"
# define WHITE "\033[37m"

# ifndef MAX_PHIL
#  define MAX_PHIL 200
# endif

/*typedef struct	s_philo
{
	int					id;
	int					last_meal;
	int					is_eating;
	int					time_to_die;
	int					time_to_sleep;
	int					time_to_eat;
	int					number_of_meals;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	struct s_table		*general;
	pthread_t			thread_id;
}				t_philo;

typedef struct s_general
{
	int					number_of_philosophers;
	int					time_to_death;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_meals;
	int					philosopher_dead;
	int					all_ate;
	int					starting_time;
	pthread_mutex_t		*fork_mutex;
	pthread_mutex_t		mutex;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_philo				*philosophers;
}				t_general;*/

//arg_checker.c
int		check_input(char **list);
int		check_for_int(long	nbr);
int		ft_check_args(char **list);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);

//philo_utils.c
int		ft_timestamp(void);


typedef struct s_philo
{
	int					id;
	int					last_meal;
	int					is_eating;
	int					time_to_die;
	int					time_to_sleep;
	int					time_to_eat;
	int					number_of_meals;
	pthread_t			thread_id;
	struct s_general	*general;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
}	t_philo;

typedef struct s_general
{
	int					number_of_philosophers;
	int					time_to_die;
	int					time_to_sleep;
	int					time_to_eat;
	int					number_of_meals;
	int					starting_time;
	int					philosopher_dead;
	pthread_mutex_t		*fork_mutex;
	pthread_mutex_t		mutex;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_philo				*philosophers;
}	t_general;


int		structs(t_general *general, char **list);
void	unlock_philosophers_mutex(t_general *philosophers);
int		mutex(t_general *general);
void	print_limits_error(void);
void	print_right_syntax(void);
void	philosophers_is_eating(t_philo *philosophers);
void	philosophers_is_sleeping(t_philo *philosophers);
void	philosophers_is_thinking(t_philo *philosophers);
void	*philosophers_routine(void *arguments);
void	begin_philosophers_routine(t_general *data);
void	free_philosophers(t_general *philosophers);
void	*begin_monitoring(void *arg);
void	join_threads(t_general *data);
void	print_message(t_philo *philosopher, char *msg);
void	ft_sleep(int time, t_philo *philosophers);
int		philosopher_is_dead(t_philo *philosophers);
long	ft_atoi_long(const char *str);

int		philosopher_is_dead(t_philo *philosophers);
int		philosopher_takes_forks(t_philo *philosophers);
int		ft_atoi(const char *str);
int		mutex(t_general *general);
int		get_time(void);
int		all_arguments_are_numbers(char **argv);
int		arguments_are_correct(int argc, char **argv);
int		no_arguments_outside_limits(char **argv);
int		right_amount_of_arguments(int argc);
int		string_is_numeric(char *str);



#endif