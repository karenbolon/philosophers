/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:20:06 by druina            #+#    #+#             */
/*   Updated: 2024/06/06 15:44:01 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

# define MAX_PHIL 200

typedef struct s_philo
{
	int				id;
	int				eating;
	int				meals_eaten;
	int				last_meal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				start_time;
	int				num_of_philos;
	int				num_times_to_eat;
	int				*dead;
	pthread_t		thread;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
}					t_philo;
typedef struct s_program
{
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	t_philo			*philos;
}					t_program;

// Main functions
//int					check_arg_content(char *arg);
//int					check_valid_args(char **argv);
void				destory_all(char *str, t_program *program,
						pthread_mutex_t *forks);

//arg_checker.c
int		check_input(char **list);
int		check_for_int(long nbr);
int		ft_check_args(char **list);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);

// Initialization
void				init_program(t_program *program, t_philo *philos);
void				init_forks(pthread_mutex_t *forks, int philo_num);
void				init_philos(t_philo *philos, t_program *program,
						pthread_mutex_t *forks, char **list);
void				init_input(t_philo *philo, char **list);

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
//int					ft_atoi(char *str);
int					ft_usleep(int microseconds);
int					ft_strlen(char *str);
void				print_message(char *str, t_philo *philo, int id);
//int				get_current_time(void);
int					ft_timestamp(void);

#endif
