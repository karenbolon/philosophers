void	init_input(t_philo *philos, char **list)
{
	philos->time_to_die = ft_atoi(list[1]);
	philos->time_to_eat = ft_atoi(list[2]);
	philos->time_to_sleep = ft_atoi(list[3]);
	philos->num_of_philos = ft_atoi(list[0]);
	if (list[4])
		philos->num_times_to_eat = (ft_atoi(list[4]));
	else
		philos->num_times_to_eat = -1;
}

//struct initialises philosopher structs and fills variables
void	init_philos(t_philo *philos, t_program *program, pthread_mutex_t *forks, char **list)
{
	int		i;

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
		i++;
	}
}

void	init_forks(pthread_mutex_t *forks, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	init_program(t_program *program, t_philo *philos)
{
	program->dead_flag = 0;
	program->philos = philos;
	pthread_mutex_init(&program->write_lock, NULL);
	pthread_mutex_init(&program->dead_lock, NULL);
	pthread_mutex_init(&program->meal_lock, NULL);
}

int	thread_create(t_program *program, pthread_mutex_t *forks)
{
	pthread_t	observer;
	int			i;

	if (pthread_create(&observer, NULL, &monitor, program->philos) != 0)
		ft_exit("Thread creation error", program, forks);
	i = 0;
	while (i < program->philos[0].num_of_philos)
	{
		if (pthread_create(&program->philos[i].thread, NULL, &philo_routine, &program->philos[i]) != 0)
			ft_exit("Thread creation error", program, forks);
		i++;
	}
	i = 0;
	if (pthread_join(observer, NULL) != 0)
		ft_exit("Thread join error", program, forks);
	while (i < program->philos[0].num_of_philos)
	{
		if (pthread_join(program->philos[i].thread, NULL) != 0)
			ft_exit("Thread join error", program, forks);
		i++;
	}
	return (0);
}


int	philosopher_dead(t_philo *philo, int time_to_die)
{
	pthread_mutex_lock(philo->meal_lock);
	if (ft_timestamp() - philo->last_meal >= time_to_die
		&& philo->eating == 0)
	{
		pthread_mutex_unlock(philo->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(philo->meal_lock);
	return (0);
}

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

int	check_if_all_ate(t_philo *philos)
{
	int	i;
	int	done;

	i = 0;
	done = 0;
	if (philos[0].num_times_to_eat == -1)
		return (0);
	while (i < philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[i].meal_lock);
		if (philos[i].meals_eaten >= philos[i].num_times_to_eat)
			done++;
		pthread_mutex_unlock(philos[i].meal_lock);
		i++;
	}
	if (done == philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos->dead = 1;
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	return (0);
}

void	*monitor(void *pointer)
{
	t_philo	*philos;

	philos = (t_philo *)pointer;
	while (1)
	{
		if (check_if_dead(philos) == 1 || check_if_all_ate(philos) == 1)
			break ;
	}
	return (pointer);
}

void	*philo_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!dead_loop(philo))
	{
		philo_is_eating(philo);
		dream(philo);
		think(philo);
	}
	return (pointer);
}

int	dead_loop(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->dead == 1)
	{
		pthread_mutex_unlock(philo->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_lock);
	return (0);
}

int	ft_timestamp(void)
{
	int					time;
	struct timeval		time_var;

	time = 0;
	if (!gettimeofday(&time_var, NULL))
		time = time_var.tv_sec * 1000 + time_var.tv_usec / 1000;
	return (time);
}

int	ft_usleep(int time)
{
	int	start;

	start = ft_timestamp();
	while ((ft_timestamp() - start) < time)
		usleep(500);
	return (0);
}

//print message checks if philo is dead and if so, prints
//message in red
void	print_message(char *s, t_philo *philo, int id)
{
	int	time;

	pthread_mutex_lock(philo->write_lock);
	time = ft_timestamp() - philo->start_time;
	if (!dead_loop(philo))
//		printf("%i %i %s\n", time, philo->id, s);
		printf("%i %i %s\n", time, id, s);
	pthread_mutex_unlock(philo->write_lock);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_exit(char *s, t_program *program, pthread_mutex_t * forks)
{
	int	i;

	i = 0;
	if (s)
	{
		while (s)
			write(1, s++, 1);
		write (1, "\n", 1);
		pthread_mutex_destroy(&program->write_lock);
		pthread_mutex_destroy(&program->meal_lock);
		pthread_mutex_destroy(&program->dead_lock);
		while (i < program->philos[0].num_of_philos)
		{
			pthread_mutex_destroy(&forks[i]);
			i++;
		}
	}
}

void	philo_is_eating(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2 == 0)
	{
		first_fork = philo->r_fork;
		second_fork = philo->l_fork;
	}
	else
	{
		first_fork = philo->l_fork;
		second_fork = philo->r_fork;	
	}
	pthread_mutex_lock(first_fork);
	print_message("has taken a fork", philo, philo->id);
	if (philo->num_of_philos == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(first_fork);
		return ;
	}
	pthread_mutex_lock(second_fork);
	print_message("has taken a fork", philo, philo->id);
//	philo->eating = 1;
	print_message("is eating", philo, philo->id);
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = ft_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_lock);
	ft_usleep(philo->time_to_eat);
//	philo->eating = 0;
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

void	think(t_philo *philo)
{
	print_message("is thinking", philo, philo->id);
}

void	dream(t_philo *philo)
{
	print_message("is sleeping", philo, philo->id);
	ft_usleep(philo->time_to_sleep);
}

int	main(int ac, char **av)
{
	char			**list;
	t_program		program;
	t_philo			philos[MAX_PHIL];
	pthread_mutex_t	forks[MAX_PHIL];

	list = NULL;
	if (ac < 5 || ac > 6)
	{
		printf("Please enter the correct parameters\n");
		return (1);
	}
	list = av + 1;
	if (ft_check_args(list) == 1)
		return (1);
	init_program(&program, philos);
	init_forks(forks, ft_atoi(list[0]));
	init_philos(philos, &program, forks, list);
	thread_create(&program, forks);
	ft_exit(NULL, &program, forks);
	return (0);
}