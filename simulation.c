/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:54:59 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/02 20:44:29 by mle-brie         ###   ########.fr       */
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
			if ((get_time(monitor->rules->start_time) - monitor->philos[i].last_meal)
			> monitor->rules->time_to_die)
			{
				// printf("monitor foresees death\n");//debug
				print_dead(monitor, monitor->rules, monitor->philos[i].id);
				return (NULL);
			}
			if (monitor->rules->meals_required != 0 && 
				monitor->philos[i].meals_eaten >= monitor->rules->meals_required)
			{
				pthread_mutex_lock(&monitor->death_lock);//test
				monitor->dead = 1;//test
				pthread_mutex_unlock(&monitor->death_lock);//test
				full++;//++ only if you're on the limit, so only if the philo is full
			}
			i++;
		}
		if (monitor->rules->meals_required != 0 
			&& full == monitor->rules->total_philos)
			return (NULL);
		usleep(20);//quick
	}
	return (NULL);
}
//notes: death flag is necessary to act upon the exit
// tips: add a "set_death_flag" to win lines
//don't forget a final printf
//you'll have to win lines, babe

bool	check_death_flag(t_monitor *monitor)
{
	bool	is_dead;

	// printf("death flag check\n");//debug
	pthread_mutex_lock(&monitor->death_lock);
	// is_dead = monitor->dead;//dead is set to 1 if dead
	if (monitor->dead == 1)
		is_dead = true;
	else
		is_dead = false;
	pthread_mutex_unlock(&monitor->death_lock);
	return (is_dead);
}

// void	launch_simulation(t_philo *philos, t_monitor *monitor, t_rules *rules)//off for test
// {
// 	int	i;

// 	i = 0;
// 	while (i < rules->total_philos)//++i?
// 	{
// 		pthread_create(&philos[i].p_thread, NULL, philo_routine, &philos[i]);
// 		i++;
// 	}
// 	pthread_create(&monitor->m_thread, NULL, monitor_routine, monitor);
// 	// pthread_join(monitor->m_thread, NULL);//here or here
// 	i = 0;
// 	while (i < rules->total_philos)//++i?
// 	{
// 		printf("test\n");//debug
// 		//it never does a join, blocks here
// 		pthread_join(philos[i].p_thread, NULL);
// 		i++;
// 	}
// 	pthread_join(monitor->m_thread, NULL);//here or here
// }

// void	launch_simulation(t_data *data)//test
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->rules->total_philos)//++i?
// 	{
// 		pthread_create(&data->philos[i].p_thread, NULL, philo_routine, &data->philos[i]);
// 		i++;
// 	}
// 	pthread_create(&data->monitor->m_thread, NULL, monitor_routine, data->monitor);
// 	// pthread_join(data->monitor->m_thread, NULL);//here or here
// 	i = 0;
// 	while (i < data->rules->total_philos)//++i?
// 	{
// 		printf("test\n");//debug
// 		//it never does a join, blocks here
// 		pthread_join(data->philos[i].p_thread, NULL);
// 		i++;
// 	}
// 	pthread_join(data->monitor->m_thread, NULL);//here or here
// 	//free array after join
// }

void	launch_simulation(t_data *data_array, int total_philos)//other test
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
		// printf("test\n");//debug
		//it never does a join, blocks here
		pthread_join(data_array[i].philos->p_thread, NULL);
		i++;
	}
	// pthread_join(data_array->monitor->m_thread, NULL);//here or here
	//free array after join
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
