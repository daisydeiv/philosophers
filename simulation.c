/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:54:59 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/05 19:08:56 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_monitor	*monitor;
	int			i;
	int			full;

	monitor = (t_monitor *)arg;
	while (!monitor->dead)
	{
		i = 0;
		full = 0;
		while (i < monitor->rules->total_philos)
		{
			check_meals(monitor, i, &full);
			i++;
		}
		if (monitor->rules->meals_required != 0
			&& full == monitor->rules->total_philos)
		{
			set_death_flag(monitor);
			printf(MAGE"All philos have eaten\n"RES);
			return (NULL);
		}
		usleep(20);//quick
	}
	release_forks(monitor->philos);//redundant, but reassuring lol
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (data->philos->id % 2 == 0)
		usleep(data->philos->rules->time_to_eat * 1000 / 2);
		//could try dividing by total_philos if there's any problem
	while (!check_death_flag(data->philos->monitor))
	{
		if (fork_routine(data->philos, data->forks))
		{
			eat_routine(data->philos);
			sleep_routine(data->philos);
			think_routine(data->philos);
		}
		else
		{
			sleep_routine(data->philos);
			think_routine(data->philos);
		}
	}
	// release_forks(data->philos);
	printf(">>philo %d exiting<<\n", data->philos->id);//debug
	return (NULL);
}

void	launch_simulation(t_data *data_array, int total_philos)
{
	int	i;

	i = 0;
	while (i < total_philos)//++i?
	{
		pthread_create(&data_array[i].philos->p_thread, NULL,
			philo_routine, &data_array[i]);
		i++;
	}
	pthread_create(&data_array->monitor->m_thread, NULL,
		monitor_routine, data_array->monitor);
	pthread_join(data_array->monitor->m_thread, NULL);//here or here
	i = 0;
	while (i < total_philos)//++i?
	{
		pthread_join(data_array[i].philos->p_thread, NULL);
		i++;
	}
}

bool	set_data_array(t_data *data)
{
	t_data	*data_array;
	int		i;

	data_array = malloc(sizeof(t_data) * data->rules->total_philos);
	if (!data_array)
		return (printf("malloc failed\n"), false);
	i = 0;
	while (i < data->rules->total_philos)
	{
		data_array[i].rules = data->rules;
		data_array[i].forks = data->forks;
		data_array[i].monitor = data->monitor;
		data_array[i].philos = &data->philos[i];
		i++;
	}
	launch_simulation(data_array, data->rules->total_philos);
	free(data_array);//free array, but not as an array
	return (true);//success
}
