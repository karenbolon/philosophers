/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 09:08:02 by kbolon            #+#    #+#             */
/*   Updated: 2024/06/06 22:53:40 by kbolon           ###   ########.fr       */
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
	int					eating;
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
	pthread_mutex_t		*l_fork;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		*write_lock;
	pthread_mutex_t		*dead_lock;
	pthread_mutex_t		*meal_lock;
}	t_philo;

typedef struct s_program
{
	int					dead_flag;
	pthread_mutex_t		dead_lock;
	pthread_mutex_t		meal_lock;
	pthread_mutex_t		write_lock;
	t_philo				*philos;
}	t_program;

/*
//arg_checker.c
int		check_input(char **list);
int		check_for_int(long nbr);
int		ft_check_args(char **list);*/
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
/*
//init.c
void	init_input(t_philo *philos, char **list);
void	init_philos(t_philo *philos, t_program *program, 
			pthread_mutex_t *forks, char **list);
//void	init_forks(pthread_mutex_t *forks, int count, int i);
void	init_forks(pthread_mutex_t *forks, int count);
void	init_program(t_program *program, t_philo *philos);
int		thread_create(t_program *program, pthread_mutex_t *forks);

//monitor.c
int		philosopher_dead(t_philo *philo, int time_to_die);
int		check_if_dead(t_philo *philos);
int		check_if_all_full(t_philo *philos);
void	*monitor(void *arg);
int		dead_loop(t_philo *philo);

*/
//philo_utils.c
int		ft_usleep(int time);
int		ft_timestamp(void);
//void	print_message(char *s, t_philo *philo);
void	print_message(char *s, t_philo *philo, int id);
int		ft_strlen(char *str);
/*
//void	ft_exit(t_program *program, pthread_mutex_t *forks);
void	ft_exit(char *s, t_program *program, pthread_mutex_t * forks);

//program.c
int		philosopher_dead(t_philo *philo, int time_to_die);
int		check_if_dead(t_philo *philos);
int		check_if_all_full(t_philo *philos);
void	*ft_monitor(void *arg);
int		dead_loop(t_philo *philo);

//philosophical_activities.c
void	philo_is_eating(t_philo *philo);
void	ft_eat_more(t_philo *philo, pthread_mutex_t	*first_fork, 
			pthread_mutex_t *second_fork);
void	*philo_routine(void *arg);
void	think(t_philo *philo);
void	dream(t_philo *philo);*/

// Main functions
int					check_arg_content(char *arg);
int					check_valid_args(char **argv);
void				destory_all(char *str, t_program *program,
						pthread_mutex_t *forks);

// Initialization
void				init_program(t_program *program, t_philo *philos);
void				init_forks(pthread_mutex_t *forks, int philo_num);
void				init_philos(t_philo *philos, t_program *program,
						pthread_mutex_t *forks, char **argv);
void				init_input(t_philo *philo, char **argv);

// Threads
int					thread_create(t_program *program, pthread_mutex_t *forks);
void				*monitor(void *pointer);
void				*philo_routine(void *pointer);

// Actions
void				eat(t_philo *philo);
void				dream(t_philo *philo);
void				think(t_philo *philo);

// Monitor utils
int					dead_loop(t_philo *philo);
int					check_if_all_ate(t_philo *philos);
int					check_if_dead(t_philo *philos);
int					philosopher_dead(t_philo *philo, int time_to_die);

// Utils
//int					ft_usleep(int microseconds);
//int					ft_strlen(char *str);
//void				print_message(char *str, t_philo *philo, int id);
//int					get_current_time(void);

#endif
