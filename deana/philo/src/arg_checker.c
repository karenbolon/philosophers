/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbolon <kbolon@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 06:35:00 by kbolon            #+#    #+#             */
/*   Updated: 2024/05/26 09:41:00 by kbolon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
