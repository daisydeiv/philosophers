/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:52:42 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/03 15:03:43 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_num(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

long	ft_atol(const char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		return (-1);
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		result = result * 10 + (str[i] - '0');
		if (result > INT_MAX)
			return (-1);
		i++;
	}
	return (result);
}

bool	parsing_args(int ac, char *av[], int *nb_philo)
{
	int	i;
	int	value;

	if (ac != 5 && ac != 6)
		return (printf("Error: wrong number of arguments\n"), false);
	(*nb_philo) = (int)ft_atol(av[1]);
	if ((int)(*nb_philo) < 1)
		return (printf("Error: not enough philos\n"), false);
	if ((int)(*nb_philo) > 200)
		return (printf("Error: exceeded number of philos\n"), false);
	i = 1;
	while (i < ac)
	{
		if (!is_num(av[i]))
			return (printf("Error: non-numeric argument for arg %d\n", i), false);
		value = ft_atol(av[i]);
		if (value < 0)
			return (printf("Error: invalid value for arg %d\n", i), false);
		i++;//almost forgot, ALWAYS
	}
	return (true);
}
