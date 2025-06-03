/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:54:59 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/03 14:03:45 by mle-brie         ###   ########.fr       */
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
			pthread_mutex_lock(&monitor->philos[i].meal_lock);//TEST
			if ((get_time(monitor->rules->start_time) - monitor->philos[i].last_meal)
			> monitor->rules->time_to_die)
			{
				pthread_mutex_unlock(&monitor->philos[i].meal_lock);//TEST/unlock if dead
				print_dead(monitor, monitor->rules, monitor->philos[i].id);
				return (NULL);
			}
			if (monitor->rules->meals_required != 0 && 
				monitor->philos[i].meals_eaten >= monitor->rules->meals_required)
				full++;//++ only if you're on the limit, so only if the philo is full/TEST
			pthread_mutex_unlock(&monitor->philos[i].meal_lock);//TEST
			i++;
		}
		if (monitor->rules->meals_required != 0 
			&& full == monitor->rules->total_philos)
		{
			pthread_mutex_lock(&monitor->death_lock);//TEST
			monitor->dead = 1;//TEST
			pthread_mutex_unlock(&monitor->death_lock);//TEST
			printf(MAGE"All philos have eaten\n"RES);//TEST
			return (NULL);
		}
		usleep(20);//quick
	}
	printf("TEST\n");
	return (NULL);
}
//notes: death flag is necessary to act upon the exit
// tips: add a "set_death_flag" to win lines
//don't forget a final printf
//you'll have to win lines, babe

void	*philo_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!check_death_flag(data->philos->monitor))
	{
		fork_routine(data->philos, data->forks);
		eat_routine(data->philos);
		sleep_routine(data->philos);
		think_routine(data->philos);
		// if (check_death_flag(data->monitor))
		// 	break ;
	}
	printf(">>philo %d exiting<<\n", data->philos->id);//debug
	return (NULL);
}

void	launch_simulation(t_data *data_array, int total_philos)
{
	int	i;

	i = 0;
	while (i < total_philos)//++i?
	{
		pthread_create(&data_array[i].philos->p_thread, NULL, philo_routine, &data_array[i]);
		i++;
	}
	pthread_create(&data_array->monitor->m_thread, NULL, monitor_routine, data_array->monitor);
	pthread_join(data_array->monitor->m_thread, NULL);//here or here
	i = 0;
	while (i < total_philos)//++i?
	{
		pthread_join(data_array[i].philos->p_thread, NULL);
		i++;
	}
	// pthread_join(data_array->monitor->m_thread, NULL);//here or here
	//free array here? naaaah?
}

bool	set_data_array(t_data *data)//other test
{
	t_data	*data_array;
	int		i;

	data_array = malloc(sizeof(t_data) * data->rules->total_philos);
	if (!data_array)
		return (printf("malloc failed\n"), false);
	i = 0;
	while (i < data->rules->total_philos)
	{
		// printf("debug test\n");//debug
		data_array[i].rules = data->rules;
		data_array[i].forks = data->forks;
		data_array[i].monitor = data->monitor;
		data_array[i].philos = &data->philos[i];
		i++;
	}
	launch_simulation(data_array, data->rules->total_philos);
	free(data_array);//free as an array then?
	return (true);//success
}
