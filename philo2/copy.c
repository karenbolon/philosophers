/*
struct initialises philosopher structs and fills variables
*/
int	init_philosophers(t_table *table, char **list)
{
	int		i;

	i = 0;
	table->philos = malloc(sizeof(t_philo) * \
		table->number_of_philosophers);
	if (!table->philos)
		return (0);
	while (i < table->number_of_philosophers)
	{
		table->philos[i].id = i + 1;
		table->philos[i].left_fork = &table->fork_mutex[i];
		table->philos[i].right_fork = &table->fork_mutex[(i + 1) % \
			table->number_of_philosophers];
		table->philos[i].table = table;
		table->philos[i].time_to_die = ft_atoi(list[1]);
		table->philos[i].time_to_eat = ft_atoi(list[2]);
		table->philos[i].time_to_sleep = ft_atoi(list[3]);
		table->philos[i].time_since_last_meal = 0;
		table->philos[i].meals_eaten = 0;
		i++;
	}
	return (1);
}

/*
struct initialises rable struct and fills variables
*/
int	init_structs(t_table *table, char **list)
{
	int	i;

	i = -1;
	table->number_of_philosophers = ft_atoi(list[0]);
	if (list[4])
		table->total_meals_to_eat = (ft_atoi(list[4]));
	else
		table->total_meals_to_eat = -1;
	table->is_dead = 0;
	table->is_full = 0;
	table->fork_mutex = malloc(sizeof(pthread_mutex_t) * \
		table->number_of_philosophers);
	if (!table->fork_mutex)
		return (0);
	while (++i < table->number_of_philosophers)
		pthread_mutex_init(&table->fork_mutex[i], NULL);
	pthread_mutex_init(&table->table_mutex, NULL);
	if (init_philosophers(table, list) == 0)
		return (0);
	return (1);
}

void	join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
}

void	begin_routine(t_table *table)
{
	int	i;

	i = 0;
	table->start_time = ft_timestamp();
	while (i < table->number_of_philosophers)
	{
		pthread_create(&table->philos[i].thread_id, NULL,
			&philosophers_routine, (void *)&table->philos[i]);
		i++;
	}
	i = 0;
	while (i < table->number_of_philosophers)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
}

/*
checks if a philosopher has died and if so, prints it in red to see easier
*/
int	check_for_dead_philos(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		philo = &table->philos[i];
		if (ft_timestamp() - philo->table->start_time > philo->time_to_die)
		{
			philo->table->is_dead = 1;
			printf("%s %d %d died\n", RED, ft_timestamp() - \
				philo->table->start_time, philo->id);
			return (0);
		}
		i++;
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

void	*begin_monitoring(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (table->is_full == 0 && table->is_dead == 0)
	{
		pthread_mutex_lock(&table->table_mutex);
		if (check_for_dead_philos(table) == 0)
		{
			usleep(500);
			pthread_mutex_unlock(&table->table_mutex);
			return (NULL);
		}
		if (table->total_meals_to_eat != -1 && \
			check_all_philosophers_done(table) == 1)
		{
			table->is_full = 1;
			pthread_mutex_unlock(&table->table_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&table->table_mutex);
		usleep(1000);
	}
	return (NULL);
}

void	*philosophers_routine(void *arguments)
{
	t_philo	*philo;

	philo = (t_philo *)arguments;
	while (philo->table->is_dead == 0 && philo->table->is_full == 0)
	{
		if (philosopher_is_dead(philo))
			return (0);
		if (!check_for_one_philo(philo))
			return (0);
		philosopher_takes_forks(philo);
		if (philosopher_is_dead(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (0);
		}
		philo_is_eating(philo);
		if (philosopher_is_dead(philo))
			return (0);
		philo_is_sleeping(philo->time_to_sleep, philo);
		if (philosopher_is_dead(philo))
			return (0);
		print_message(philo, "is thinking");
	}
	return (0);
}

int	check_for_one_philo(t_philo *philo)
{
	if (philo->table->number_of_philosophers == 1)
	{
		print_message(philo, "has taken a fork");
		philo_is_sleeping(philo->time_to_die + 1, philo);
		philo->table->is_dead = 1;
		print_message(philo, "died");
		return (0);
	}
	return (1);
}

/*
fcn gets the time, fills a time struct: tv_sec->seconds, tv_usec->milliseconds
we need to convert to add together
gettimeofday returns 0 if successful
*/
int	ft_timestamp(void)
{
	int					time;
	struct timeval		time_var;

	time = 0;
	if (!gettimeofday(&time_var, NULL))
		time = time_var.tv_sec * 1000 + time_var.tv_usec / 1000;
	return (time);
}

/*
print message checks if philo is dead and if so, prints
message in red
*/
void	print_message(t_philo *philo, char *s)
{
	if (!(philosopher_is_dead(philo)))
	{
		pthread_mutex_lock(&philo->table->table_mutex);
		printf("%i %i %s\n", ft_timestamp() - \
			philo->table->start_time, philo->id, s);
		pthread_mutex_unlock(&(philo->table->table_mutex));
	}
	if (philosopher_is_dead(philo))
	{
		printf("%s %i %i %s\n", RED, ft_timestamp() - \
			philo->table->start_time, philo->id, s);
		pthread_mutex_unlock(&(philo->table->table_mutex));
	}
}

void	freedom_function(t_table *table)
{
	if (table->philos)
		free(table->philos);
	if (table->fork_mutex)
		free(table->fork_mutex);
}

void	philo_is_eating(t_philo *philo)
{
	print_message(philo, "is eating");
	pthread_mutex_lock(&philo->table->table_mutex);
	philo->time_since_last_meal = ft_timestamp() - \
		philo->table->start_time;
	philo->time_to_die = philo->time_since_last_meal + \
		philo->time_to_die;
	pthread_mutex_unlock(&philo->table->table_mutex);
	philo_is_sleeping(philo->time_to_eat, philo);
	pthread_mutex_lock(&philo->table->table_mutex);
	if (philo->meals_eaten != -1)
		philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->table_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_is_sleeping(int time, t_philo *philo)
{
	int	start;

	start = ft_timestamp();
	print_message(philo, "is sleeping");
	while ((ft_timestamp() - start) < time && !philosopher_is_dead(philo))
		usleep(100);
}

int	philosopher_is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->table_mutex);
	if (philo->table->is_dead == 1)
	{
		pthread_mutex_unlock(&philo->table->table_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->table_mutex);
	return (0);
}

int	philosopher_takes_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		if (!philosopher_is_dead(philo))
			print_message(philo, "has taken a fork");
		if (pthread_mutex_lock(philo->left_fork) != 0)
		{
			pthread_mutex_unlock(philo->right_fork);
			return (1);
		}
		if (!philosopher_is_dead(philo))
			print_message(philo, "has taken a fork");
	}
	else
		if (odd_philosophers(philo))
			return (1);
	return (0);
}

int	odd_philosophers(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (!philosopher_is_dead(philo))
		print_message(philo, "has taken a fork");
	if (philo->table->number_of_philosophers == 1)
	{
		philo->table->is_dead = 1;
		philosopher_is_dead(philo);
	}
	if (pthread_mutex_lock(philo->right_fork) != 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	if (!philosopher_is_dead(philo))
		print_message(philo, "has taken a fork");
	return (0);
}

int	main(int ac, char **av)
{
	char		**list;
	t_table		table;
	pthread_t	monitor_thread;

	list = NULL;
	if (ac < 5 || ac > 6)
	{
		printf("Please enter the correct parameters\n");
		return (1);
	}
	list = av + 1;
	if (ft_check_args(list) == 1)
		return (1);
	if (init_structs(&table, list) == 0)
		return (1);
	pthread_create(&monitor_thread, NULL, &begin_monitoring, (void *)&table);
	begin_routine(&table);
	pthread_join(monitor_thread, NULL);
	join_threads(&table);
	freedom_function(&table);
	return (0);
}