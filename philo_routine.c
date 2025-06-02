/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:56:16 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/02 20:37:46 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fork_selection_process(t_philo *philos, t_fork *forks)
{
	int	index;

	index = philos->id - 1;//because forks and philos are not the same
	//select fork
	if (philos->id % 2 == 0)
	{
		philos->l_fork = &forks[index];
		philos->r_fork = &forks[(index + 1) % philos->rules->total_philos];
	}
	else
	{
		philos->r_fork = &forks[index];
		philos->l_fork = &forks[(index + 1) % philos->rules->total_philos];
	}
}

void	fork_routine(t_philo *philos, t_fork *forks)
{
	if (check_death_flag(philos->monitor))
		return ;//leave
	fork_selection_process(philos, forks);
	//=> invert left and right to avoid weird deadlocks
	
	//left fork process
	pthread_mutex_lock(&philos->l_fork->mutex);
	if (check_death_flag(philos->monitor))
	{
		pthread_mutex_unlock(&philos->l_fork->mutex);
		return ;//leave
	}
	print_fork(philos->monitor, philos->rules, philos->id);

	//right fork process
	if (check_death_flag(philos->monitor))
	{
		pthread_mutex_unlock(&philos->l_fork->mutex);
		return ;//leave	
	}
	pthread_mutex_lock(&philos->r_fork->mutex);
	print_fork(philos->monitor, philos->rules, philos->id);
}

void	eat_routine(t_philo *philos)
{
	if (check_death_flag(philos->monitor))
	{
		pthread_mutex_unlock(&philos->l_fork->mutex);
		pthread_mutex_unlock(&philos->r_fork->mutex);
		return ;//leave	
	}
	pthread_mutex_lock(&philos->meal_lock);
	print_eat(philos->monitor, philos->rules, philos->id);//should I start with the printing states BEFORE all the locks and all???
	if (!check_death_flag(philos->monitor))
	{
		philos->last_meal = get_time(philos->rules->start_time);
		philos->meals_eaten++;
	}
	smart_usleep(philos->rules->time_to_eat, philos->rules->start_time, philos->monitor);//mod
	pthread_mutex_unlock(&philos->meal_lock);
	pthread_mutex_unlock(&philos->l_fork->mutex);//same
	pthread_mutex_unlock(&philos->r_fork->mutex);//forks off after eating
}

void	sleep_routine(t_philo *philos)
{
	if (check_death_flag(philos->monitor))
		return ;//leave
	smart_usleep(philos->rules->time_to_sleep, philos->rules->start_time, philos->monitor);//mod
	print_sleep(philos->monitor, philos->rules, philos->id);
}

void	think_routine(t_philo *philos)
{
	if (check_death_flag(philos->monitor))
		return ;//leave
	// smart_usleep(philos->rules->time_to_think****, philos->rules->start_time);
	smart_usleep(100, philos->rules->start_time, philos->monitor);//mod
	// ***more like, while l_fork & r_fork not available
	print_think(philos->monitor, philos->rules, philos->id);
}

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

/*
NOTES: def have to go back on all of this lol
for forks, if second fork block, wait
really have to do solo philo outside, as a specific edge case, uh
for think, maybe either small smart_usleep, or loop for the forks?
But forks need to have a check that you do it only if *both* are available

2:
the trylock function if amazing, if it returns 0, it means it can do it
so if trylock forks == 0, you should unlock them
*/