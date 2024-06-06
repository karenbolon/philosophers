

int	check_input(char **list)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '\0')
		{
			if (list[i][j] < 48 || list[i][j] > 57)
			{
				printf("Please use only positive integers\n");
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	check_for_int(long nbr)
{
	if (nbr <= 0 || nbr >= INT_MAX)
	{
		printf("Please enter a positive integer\n");
		return (1);
	}
	return (0);
}

int	ft_check_args(char **list)
{
	long	nbr_philo;
	int		i;

	i = 0;
	if (check_input(list) == 1)
		return (1);
	while (list[i])
	{
		if (ft_atol(list[i]) == 1)
			return (1);
		i++;
	}
	nbr_philo = ft_atoi(list[0]);
	if (nbr_philo > MAX_PHIL || nbr_philo <= 0)
	{
		printf("Please enter more than 0 and less than 200 Philosophers\n");
		return (1);
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	unsigned int	i;
	int				sign;
	int				result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == 45)
	{
		sign *= -1;
		i++;
	}
	else if (str[i] == 43)
		i++;
	while (str[i] >= 48 && str[i] <= 57 && str[i] != '\0')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

long	ft_atol(const char *str)
{
	long	i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == 45)
	{
		sign *= -1;
		i++;
	}
	else if (str[i] == 43)
		i++;
	while (str[i] >= 48 && str[i] <= 57 && str[i] != '\0')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (check_for_int(result * sign) == 1)
		return (1);
	return (0);
}

void	init_input(t_philo *philo, char **list)
{
	philo->time_to_die = ft_atoi(list[1]);
	philo->time_to_eat = ft_atoi(list[2]);
	philo->time_to_sleep = ft_atoi(list[3]);
	philo->num_of_philos = ft_atoi(list[0]);
	if (list[4])
		philo->num_times_to_eat = ft_atoi(list[4]);
	else
		philo->num_times_to_eat = -1;
}

// Initializing the philosophers

void	init_philos(t_philo *philos, t_program *program, pthread_mutex_t *forks,
		char **list)
{
	int	i;

	i = 0;
	while (i < ft_atoi(list[0]))
	{
		philos[i].id = i + 1;
		philos[i].eating = 0;
		philos[i].meals_eaten = 0;
		init_input(&philos[i], list);
		philos[i].start_time = ft_timestamp();
		philos[i].last_meal = ft_timestamp();
		philos[i].write_lock = &program->write_lock;
		philos[i].dead_lock = &program->dead_lock;
		philos[i].meal_lock = &program->meal_lock;
		philos[i].dead = &program->dead_flag;
		philos[i].l_fork = &forks[i];
		if (i == 0)
			philos[i].r_fork = &forks[philos[i].num_of_philos - 1];
		else
			philos[i].r_fork = &forks[i - 1];
//		init_input(&philos[i], list);
		i++;
	}
}

// Initializing the forks mutexes

void	init_forks(pthread_mutex_t *forks, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

// Initializing the program structure

void	init_program(t_program *program, t_philo *philos)
{
	program->dead_flag = 0;
	program->philos = philos;
	pthread_mutex_init(&program->write_lock, NULL);
	pthread_mutex_init(&program->dead_lock, NULL);
	pthread_mutex_init(&program->meal_lock, NULL);
}

void	print_message(char *str, t_philo *philo, int id)
{
	int	time;

	pthread_mutex_lock(philo->write_lock);
	time = ft_timestamp() - philo->start_time;
	if (!dead_loop(philo))
		printf("%d %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->write_lock);
}

// Checks if the philosopher is dead

int	philosopher_dead(t_philo *philo, int time_to_die)
{
	pthread_mutex_lock(philo->meal_lock);
	if (ft_timestamp() - philo->last_meal >= time_to_die
		&& philo->eating == 0)
		return (pthread_mutex_unlock(philo->meal_lock), 1);
	pthread_mutex_unlock(philo->meal_lock);
	return (0);
}

// Check if any philo died

int	check_if_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].num_of_philos)
	{
		if (philosopher_dead(&philos[i], philos[i].time_to_die))
		{
			print_message("died", &philos[i], philos[i].id);
			pthread_mutex_lock(philos[0].dead_lock);
			*philos->dead = 1;
			pthread_mutex_unlock(philos[0].dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

// Checks if all the philos ate the num_of_meals

int	check_if_all_ate(t_philo *philos)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].num_times_to_eat == -1)
		return (0);
	while (i < philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[i].meal_lock);
		if (philos[i].meals_eaten >= philos[i].num_times_to_eat)
			finished_eating++;
		pthread_mutex_unlock(philos[i].meal_lock);
		i++;
	}
	if (finished_eating == philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos->dead = 1;
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	return (0);
}

// Monitor thread routine

void	*monitor(void *pointer)
{
	t_philo	*philos;

	philos = (t_philo *)pointer;
	while (1)
		if (check_if_dead(philos) == 1 || check_if_all_ate(philos) == 1)
			break ;
	return (pointer);
}

// Think routine funtion

void	think(t_philo *philo)
{
	print_message("is thinking", philo, philo->id);
}

// Dream routine funtion

void	dream(t_philo *philo)
{
	print_message("is sleeping", philo, philo->id);
	ft_usleep(philo->time_to_sleep);
}

// Eat routine funtion

//mine
void eat(t_philo *philo)
{
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    if (philo->id % 2 == 0) {
        first_fork = philo->r_fork;
        second_fork = philo->l_fork;
    } else {
        first_fork = philo->l_fork;
        second_fork = philo->r_fork;
    }

    pthread_mutex_lock(first_fork);
    print_message("has taken a fork", philo, philo->id);
    if (philo->num_of_philos == 1)
    {
        ft_usleep(philo->time_to_die);
        pthread_mutex_unlock(first_fork);
        return;
    }
    pthread_mutex_lock(second_fork);
    print_message("has taken a fork", philo, philo->id);
//    philo->eating = 1;
    print_message("is eating", philo, philo->id);
    pthread_mutex_lock(philo->meal_lock);
    philo->last_meal = ft_timestamp();
    philo->meals_eaten++;
    pthread_mutex_unlock(philo->meal_lock);
    ft_usleep(philo->time_to_eat);
//    philo->eating = 0;
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}
if the value of dead_flag changed

int	dead_loop(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->dead == 1)
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	pthread_mutex_unlock(philo->dead_lock);
	return (0);
}

// Thread routine

void	*philo_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!dead_loop(philo))
	{
		eat(philo);
		dream(philo);
		think(philo);
	}
	return (pointer);
}

// Creates all the threads

int	thread_create(t_program *program, pthread_mutex_t *forks)
{
	pthread_t	observer;
	int			i;

	if (pthread_create(&observer, NULL, &monitor, program->philos) != 0)
		destory_all("Thread creation error", program, forks);
	i = 0;
	while (i < program->philos[0].num_of_philos)
	{
		if (pthread_create(&program->philos[i].thread, NULL, &philo_routine,
				&program->philos[i]) != 0)
			destory_all("Thread creation error", program, forks);
		i++;
	}
	i = 0;
	if (pthread_join(observer, NULL) != 0)
		destory_all("Thread join error", program, forks);
	while (i < program->philos[0].num_of_philos)
	{
		if (pthread_join(program->philos[i].thread, NULL) != 0)
			destory_all("Thread join error", program, forks);
		i++;
	}
	return (0);
}

int	ft_strlen(char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

// Destroys all the mutexes

void	destory_all(char *str, t_program *program, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		while (*str)
		{
//		write(2, str, ft_strlen(str));
//		write(2, "\n", 1);
			write(1, str++, 1);
		}
		write (1, "\n", 1);
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

int	ft_usleep(int milliseconds)
{
	int	start;

	start = ft_timestamp();
	while ((ft_timestamp() - start) < milliseconds)
		usleep(500);
	return (0);
}

//mine
int	ft_timestamp(void)
{
	int					time;
	struct timeval		time_var;

	time = 0;
	if (!gettimeofday(&time_var, NULL))
		time = time_var.tv_sec * 1000 + time_var.tv_usec / 1000;
	return (time);
}

int	main(int argc, char **argv)
{
	t_program		program;
	t_philo			philos[MAX_PHIL];
	pthread_mutex_t	forks[MAX_PHIL];
	char			**list;

	list = NULL;
	if (argc < 5 || argc > 6)
		return (write(2, "Wrong argument count\n", 22), 1);
	list = argv + 1;
	if (ft_check_args(list) == 1)
		return (1);
	init_program(&program, philos);
	init_forks(forks, ft_atoi(list[0]));
	init_philos(philos, &program, forks, list);
	thread_create(&program, forks);
	destory_all(NULL, &program, forks);
	return (0);
}
