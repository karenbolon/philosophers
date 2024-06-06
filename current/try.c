
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
struct initialises table struct and fills variables
"sets the table and lays the forks"
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
	table->start_time = ft_timestamp();
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
//	table->start_time = ft_timestamp();
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

/*
this functions tells the philosopher what to do and checks
if they died inbetween actions

forks must be locked before a philo can "pick up" and use, and unlock when
"put down".  This prevents philos from using same forks
*/
void	*philosophers_routine(void *arguments)
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
}

int	check_for_one_philo(t_philo *philo)
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

/*
print message checks if philo is dead
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
		pthread_mutex_lock(&philo->table->table_mutex);
		printf("%i %i %s\n", ft_timestamp() + 1 - \
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
		usleep(500);//was 100
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
	usleep(500);
	pthread_mutex_lock(philo->left_fork);
	if (!philosopher_is_dead(philo))
		print_message(philo, "has taken a fork");
	if (pthread_mutex_lock(philo->right_fork) != 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	if (!philosopher_is_dead(philo))
		print_message(philo, "has taken a fork");
	return (0);
}
