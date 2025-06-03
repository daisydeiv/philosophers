/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:58:08 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/03 11:59:53 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_one(t_data *data)
{
	pthread_mutex_lock(&data->philos->l_fork->mutex);
	print_fork(data->monitor, data->rules, data->philos->id);
	smart_usleep(data->rules->time_to_die, data->rules->start_time, data->monitor);//mod
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

//the helper function of monitor here