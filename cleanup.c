/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 13:41:01 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/05 18:03:04 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	release_forks(t_philo *philos)
{
	if (philos->has_left_fork)
	{
		pthread_mutex_unlock(&philos->l_fork->mutex);
		philos->has_left_fork = false;
	}
	if (philos->has_right_fork)
	{
		pthread_mutex_unlock(&philos->r_fork->mutex);
		philos->has_right_fork = false;
	}
}

void	free_forks(t_fork *forks, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->total_philos)//++i?
	{
		pthread_mutex_destroy(&forks[i].mutex);//no alloc
		i++;
	}
	free(forks);//dynamic alloc
}

void	free_philos(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].rules->total_philos)
	{
		pthread_mutex_destroy(&philos[i].meal_lock);//no alloc
		i++;
	}
	free(philos);//dynamic alloc
}

void	free_monitor(t_monitor *monitor)
{
	pthread_mutex_destroy(&monitor->death_lock);//no alloc
	pthread_mutex_destroy(&monitor->write_lock);//no alloc
	free(monitor);//dynamic alloc
}

void	free_all(t_data *data)
{
	if (data->forks)
		free_forks(data->forks, data->rules);
	if (data->philos)
		free_philos(data->philos);
	if (data->monitor)
		free_monitor(data->monitor);
	if (data->rules)//last coz I use it in forks & philos
		free(data->rules);//only alloc, no mutex
	free(data);
}
