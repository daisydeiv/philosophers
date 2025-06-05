/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:58:08 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/05 19:28:51 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_one(t_data *data)
{
	pthread_mutex_lock(&data->philos->l_fork->mutex);
	print_fork(data->monitor, data->rules, data->philos->id);
	smart_usleep(data->rules->time_to_die,
		data->rules->start_time, data->monitor);
	print_dead(data->monitor, data->rules, data->philos->id);
	pthread_mutex_unlock(&data->philos->l_fork->mutex);
}

bool	check_death_flag(t_monitor *monitor)
{
	bool	is_dead;

	pthread_mutex_lock(&monitor->death_lock);
	if (monitor->dead == 1)
		is_dead = true;
	else
		is_dead = false;
	pthread_mutex_unlock(&monitor->death_lock);
	return (is_dead);
}

bool	die_and_release(t_philo *philos)//test
{
	if (check_death_flag(philos->monitor))
	{
		release_forks(philos);
		return (false);	
	}
	return (true);
}

void	set_death_flag(t_monitor *monitor)
{
	pthread_mutex_lock(&monitor->death_lock);
	monitor->dead = 1;
	pthread_mutex_unlock(&monitor->death_lock);
}

void	check_meals(t_monitor *monitor, int i, int *full)
{
	pthread_mutex_lock(&monitor->philos[i].meal_lock);
	if ((get_time(monitor->rules->start_time) - monitor->philos[i].last_meal)
		> monitor->rules->time_to_die)
	{
		pthread_mutex_unlock(&monitor->philos[i].meal_lock);
		print_dead(monitor, monitor->rules, monitor->philos[i].id);
		// return (NULL);//nor return NULL, but should be set elsewhere
	}
	if (monitor->rules->meals_required != 0
		&& monitor->philos[i].meals_eaten >= monitor->rules->meals_required)
		(*full)++;
	pthread_mutex_unlock(&monitor->philos[i].meal_lock);
}

/*
NOTES
die_and_release needs to be implemented
check_meals need to deal with the NULL
*/