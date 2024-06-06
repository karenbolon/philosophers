# define RED "\033[1;31m"
# define WHITE "\033[37m"
# define BLUE "\033[34m"

# ifndef MAX_PHIL
#  define MAX_PHIL 200
# endif

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
	int					is_full;
	pthread_mutex_t		*fork_mutex;
	pthread_mutex_t		mutex;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_philo				*philosophers;
}	t_general;

//checked
int	initialise_philosophers(t_general *general)
{
	int		i;

	i = 0;
	general->philosophers = malloc(sizeof(t_philo) * \
		general->number_of_philosophers);
	if (!general->philosophers)
		return (FAILURE);
	while (i < general->number_of_philosophers)
	{
		general->philosophers[i].id = i + 1;
		general->philosophers[i].left_fork = &general->fork_mutex[i];
		general->philosophers[i].right_fork = &general->fork_mutex[(i + 1) % 
			general->number_of_philosophers];
		general->philosophers[i].general = general;
		general->philosophers[i].time_to_die = general->time_to_die;
		general->philosophers[i].time_to_eat = general->time_to_eat;
		general->philosophers[i].time_to_sleep = general->time_to_sleep;
		general->philosophers[i].last_meal = 0;
		general->philosophers[i].is_eating = 0;
		general->philosophers[i].number_of_meals = 0;
		i++;
	}
	return (SUCCESS);
}
//checked
int		structs(t_general *general, char **list)
{
	general->number_of_philosophers = ft_atoi(list[0]);
	general->time_to_die = ft_atoi(list[1]);
	general->time_to_eat = ft_atoi(list[2]);
	general->time_to_sleep = ft_atoi(list[3]);
	if (list[4])
		general->number_of_meals  = (ft_atoi(list[4]));
	else
		general->number_of_meals = -1;
	general->philosopher_dead = 0;
	general->is_full = 0;
	if (mutex(general) == FAILURE)
		return (FAILURE);
	if (initialise_philosophers(general) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
//checked
int	mutex(t_general *general)
{
	int	i;

	i = -1;
	general->fork_mutex = malloc(sizeof(pthread_mutex_t) * \
		general->number_of_philosophers);
	if (!general->fork_mutex)
		return (FAILURE);
	while (++i < general->number_of_philosophers)
		pthread_mutex_init(&general->fork_mutex[i], NULL);
	pthread_mutex_init(&general->mutex, NULL);
	return (SUCCESS);
}//checked

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
            ft_sleep(philosophers->general->time_to_die, philosophers);
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
//checked
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

//checked
int	ft_timestamp(void)
{
	struct timeval		time_var;

	gettimeofday(&time_var, NULL);
	return ((time_var.tv_sec * 1000) + (time_var.tv_usec/1000));
}


void print_message(t_philo *philosopher, char *str)
{
    if (!philosopher_is_dead(philosopher))
    {
        pthread_mutex_lock(&philosopher->general->mutex);
        printf("%d %d %s\n", ft_timestamp() - philosopher->general->starting_time, philosopher->id, str);
        pthread_mutex_unlock(&philosopher->general->mutex);
    }
}


//checked
void	philosophers_is_eating(t_philo *philosophers)
{
	print_message(philosophers, "is eating");
	pthread_mutex_lock(&philosophers->general->mutex);
	philosophers->last_meal = ft_timestamp() - \
		philosophers->general->starting_time;
	philosophers->time_to_die = philosophers->last_meal + \
		philosophers->general->time_to_die;
	pthread_mutex_unlock(&philosophers->general->mutex);
	ft_sleep(philosophers->general->time_to_eat, philosophers);
	pthread_mutex_lock(&philosophers->general->mutex);
	if (philosophers->number_of_meals != -1)
	{
		philosophers->number_of_meals++;
//		if (philosophers->number_of_meals >= philosophers->general->number_of_meals)
//			philosophers->general->is_full = 1;
	}
	pthread_mutex_unlock(&philosophers->general->mutex);
	pthread_mutex_unlock(philosophers->left_fork);
	pthread_mutex_unlock(philosophers->right_fork);
}
//checked
void	philosophers_is_sleeping(t_philo *philosophers)
{
	print_message(philosophers, "is sleeping");
	ft_sleep(philosophers->general->time_to_sleep, philosophers);
}
//checked
void	philosophers_is_thinking(t_philo *philosophers)
{
	print_message(philosophers, "is thinking");
}
//checked
int	philosopher_is_dead(t_philo *philosophers)
{
	pthread_mutex_lock(&philosophers->general->mutex);
	if (philosophers->general->philosopher_dead == 1)
	{
		pthread_mutex_unlock(&philosophers->general->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philosophers->general->mutex);
	return (0);
}

//checked
/*int	philosopher_takes_forks(t_philo *philosophers)
{
	if (philosophers->id %2 == 0)
		pthread_mutex_lock(philosophers->right_fork);
	else
		pthread_mutex_lock(philosophers->left_fork);
	if (!philosopher_is_dead(philosophers))
		print_message(philosophers, "has taken a fork");
	if (philosophers->id %2 == 0)
	{
		if (pthread_mutex_lock(philosophers->left_fork) != 0)
		{
			pthread_mutex_unlock(philosophers->right_fork);
			return (1);
		}
		if (!philosopher_is_dead(philosophers))
			print_message(philosophers, "has taken a fork");
	}
	else
	{
		if (pthread_mutex_lock(philosophers->right_fork) != 0)
		{
			pthread_mutex_unlock(philosophers->left_fork);
			return (1);
		}
		if (!philosopher_is_dead(philosophers))
			print_message(philosophers, "has taken a fork");
	}
	return (0);
}*/

int philosopher_takes_forks(t_philo *philosophers)
{
    if (philosophers->id % 2 == 0)
    {
        pthread_mutex_lock(philosophers->right_fork);
        if (!philosopher_is_dead(philosophers))
            print_message(philosophers, "has taken a fork");
        pthread_mutex_lock(philosophers->left_fork);
        if (!philosopher_is_dead(philosophers))
            print_message(philosophers, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philosophers->left_fork);
        if (!philosopher_is_dead(philosophers))
            print_message(philosophers, "has taken a fork");
//		if (philosophers->general->number_of_philosophers == 1)
//			philosophers->general->philosopher_dead = 1;
        pthread_mutex_lock(philosophers->right_fork);
        if (!philosopher_is_dead(philosophers))
            print_message(philosophers, "has taken a fork");
    }
    return 0;
}

int	main(int ac, char **av)
{
	char			**list;
	t_general		general;
	pthread_t		monitor_thread;

	list = NULL;
	if (ac < 5 || ac > 6)
	{
		printf("Please enter the correct parameters\n");
		return (1);
	}
	list = av + 1;
	if (ft_check_args(list) == 1)
		return (1);
	if (structs(&general, list) == FAILURE)
		return (1);
/*	if (general.number_of_philosophers == 1)
	{
		handle_one_philosopher(&general.philosophers[0]);
		free_philosophers(&general);
		return (0);
	}*/
	pthread_create(&monitor_thread, NULL, &begin_monitoring, (void *)&general);
	begin_philosophers_routine(&general);
	pthread_join(monitor_thread, NULL);
//		begin_monitoring(&general);
	join_threads(&general);
	free_philosophers(&general);
	return (0);
}
