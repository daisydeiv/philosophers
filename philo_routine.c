/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:56:16 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/03 14:13:41 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	fork_selection_process(t_philo *philos, t_fork *forks)
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
		return ;
	fork_selection_process(philos, forks);
	//=> invert left and right to avoid weird deadlocks
	
	//left fork process
	pthread_mutex_lock(&philos->l_fork->mutex);
	philos->has_left_fork = true;
	if (check_death_flag(philos->monitor))
	{
		release_forks(philos);
		return ;//leave
	}
	print_fork(philos->monitor, philos->rules, philos->id);

	//right fork process
	if (check_death_flag(philos->monitor))
	{
		release_forks(philos);
		return ;//leave	
	}
	pthread_mutex_lock(&philos->r_fork->mutex);
	philos->has_right_fork = true;
	print_fork(philos->monitor, philos->rules, philos->id);
	if (check_death_flag(philos->monitor))//added block
	{
		release_forks(philos);//add
		return ;//leave	
	}//added block
}

void	eat_routine(t_philo *philos)
{
	if (check_death_flag(philos->monitor))
	{
		release_forks(philos);
		return ;//leave	
	}
	pthread_mutex_lock(&philos->meal_lock);
	// print_eat(philos->monitor, philos->rules, philos->id);//should I start with the printing states BEFORE all the locks and all???
	if (!check_death_flag(philos->monitor))
	{
		philos->last_meal = get_time(philos->rules->start_time);
		philos->meals_eaten++;
		print_eat(philos->monitor, philos->rules, philos->id);//print only if not dead
	}
	smart_usleep(philos->rules->time_to_eat, philos->rules->start_time, philos->monitor);//mod
	pthread_mutex_unlock(&philos->meal_lock);
	pthread_mutex_unlock(&philos->l_fork->mutex);//forks off after eating
	philos->has_left_fork = false;
	pthread_mutex_unlock(&philos->r_fork->mutex);//same
	philos->has_right_fork = false;
}

void	sleep_routine(t_philo *philos)
{
	if (check_death_flag(philos->monitor))
		return ;
	smart_usleep(philos->rules->time_to_sleep, philos->rules->start_time, philos->monitor);//mod
	print_sleep(philos->monitor, philos->rules, philos->id);
}

void	think_routine(t_philo *philos)
{
	if (check_death_flag(philos->monitor))
		return ;
	smart_usleep(100, philos->rules->start_time, philos->monitor);
	print_think(philos->monitor, philos->rules, philos->id);
}

/*
NOTES: def have to go back on all of this lol
for forks, if second fork block, wait
really have to do solo philo outside, as a specific edge case, uh
for think, maybe either small smart_usleep, or loop for the forks?
But forks need to have a check that you do it only if *both* are available

2:
the trylock function if amazing, if it returns 0, it means it can't do it
so if trylock forks == 0, you should unlock them
*/