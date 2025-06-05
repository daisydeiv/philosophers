/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:56:16 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/05 12:20:065 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fork_order(t_philo *philos)//test
{
	t_fork *tmp;

	if (philos->l_fork > philos->r_fork)
	{
		tmp = philos->l_fork;
		philos->l_fork = philos->r_fork;
		philos->r_fork = tmp;
	}
}

bool	fork_routine(t_philo *philos, t_fork *forks)
{
	forks = forks;
	if (check_death_flag(philos->monitor))
		return (false);

	if (philos->has_right_fork == true || philos->has_left_fork == true)
	{
		printf(MAGE"philo %d can't take left or right fork %d\n"RES, philos->id, philos->l_fork->id);//debug
		return (false);
	}
	
	fork_order(philos);//test
	//=> invert left and right to avoid weird deadlocks

	//left fork process
	if (check_death_flag(philos->monitor))
		return (false);//add
	pthread_mutex_lock(&philos->l_fork->mutex);
	philos->has_left_fork = true;
	if (check_death_flag(philos->monitor))
	{
		release_forks(philos);
		return (false);
	}
	print_fork(philos->monitor, philos->rules, philos->id);

	//right fork process
	if (check_death_flag(philos->monitor))
	{
		release_forks(philos);
		return (false);	
	}
	pthread_mutex_lock(&philos->r_fork->mutex);
	philos->has_right_fork = true;
	if (check_death_flag(philos->monitor))//added block
	{
		release_forks(philos);//add
		return (false);//add	
	}//added block
	print_fork(philos->monitor, philos->rules, philos->id);//mod

	return (true);
}

void	eat_routine(t_philo *philos)
{
	if (check_death_flag(philos->monitor))
	{
		release_forks(philos);
		return ;
	}
	pthread_mutex_lock(&philos->meal_lock);
	if (!check_death_flag(philos->monitor))
	{
		philos->last_meal = get_time(philos->rules->start_time);
		philos->meals_eaten++;
		print_eat(philos->monitor, philos->rules, philos->id);//print only if not dead
	}
	smart_usleep(philos->rules->time_to_eat, philos->rules->start_time, philos->monitor);
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
	print_sleep(philos->monitor, philos->rules, philos->id);
	smart_usleep(philos->rules->time_to_sleep, philos->rules->start_time, philos->monitor);
}

void	think_routine(t_philo *philos)
{
	// long	time_to_think;

	// time_to_think = philos->rules->time_to_die
	// 				- philos->rules->time_to_eat
	// 				- philos->rules->time_to_sleep;
	// if (time_to_think < 0)
	// 	time_to_think = 0;
	if (check_death_flag(philos->monitor))
		return ;
	print_think(philos->monitor, philos->rules, philos->id);//mod
	// smart_usleep(time_to_think, philos->rules->start_time, philos->monitor);
	usleep(500);
	// if (check_death_flag(philos->monitor))
	// 	return ;//test
	// print_think(philos->monitor, philos->rules, philos->id);
}
