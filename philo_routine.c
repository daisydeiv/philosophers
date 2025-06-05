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

// static void	fork_selection_process(t_philo *philos, t_fork *forks)
// {
// 	philo->fork[0] = philo->id;
// 	philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
// 	if (philo->id % 2)
// 	{
// 		philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
// 		philo->fork[1] = philo->id;
// 	}
	
// 	-1 % 5 == 4

// 	id = 0;
// 	philo 1 = L 0 - R 1

// 	5 % 5 = 0

// 	-1 % 5 != 4
	
// 	int	index;
// 	// fork = 0, 1, 2, 3, 4
// 	// philos = 1, 2, 3, 4, 5

// 	index = philos->id - 1;//because forks and philos are not the same
// 	//select fork

// 	if (philos->id == 1)
// 	{
// 		printf("test\n");//debug
// 		philos->l_fork = &forks[philos->rules->total_philos - 1];//philo 1 last fork
// 		printf("left fork is %d\n", philos->l_fork->id);//debug
// 		philos->r_fork = &forks[index];//philo 1 fork = 0
// 		printf("right fork is %d\n", philos->r_fork->id);//debug
// 	}
// 	else
// 	{
// 		philos->l_fork = &forks[index - 1];//philo 2 fork 0
// 		printf("left fork is %d\n", philos->l_fork->id);//debug
// 		philos->r_fork = &forks[index];//philo 2, fork 1
// 		printf("right fork is %d\n", philos->r_fork->id);//debug
// 	}
	
// 	// if (philos->id % 2 == 0)
// 	// {//test
// 	// 	philos->l_fork = &forks[index];//for philo 1, index 0, takes 0
// 	// 	philos->r_fork = &forks[(index + 1) % philos->rules->total_philos];//for philo 1, index 0, takes ..
// 	// }
// 	// else
// 	// {
// 	// 	philos->r_fork = &forks[index];
// 	// 	philos->l_fork = &forks[(index + 1) % philos->rules->total_philos];
// 	// }
// }

// void	fork_routine(t_philo *philos, t_fork *forks)
// {
// 	if (check_death_flag(philos->monitor))
// 		return ;
// 	fork_selection_process(philos, forks);
// 	//=> invert left and right to avoid weird deadlocks
	
// 	//left fork process
// 	if (check_death_flag(philos->monitor))
// 		return ;//add
// 	pthread_mutex_lock(&philos->l_fork->mutex);
// 	philos->has_left_fork = true;
// 	if (check_death_flag(philos->monitor))
// 	{
// 		release_forks(philos);
// 		return ;//leave
// 	}
// 	print_fork(philos->monitor, philos->rules, philos->id);

// 	//right fork process
// 	if (check_death_flag(philos->monitor))
// 	{
// 		release_forks(philos);
// 		return ;//leave	
// 	}
// 	pthread_mutex_lock(&philos->r_fork->mutex);
// 	philos->has_right_fork = true;
// 	if (check_death_flag(philos->monitor))//added block
// 	{
// 		release_forks(philos);//add
// 		return ;//leave	
// 	}//added block
// 	print_fork(philos->monitor, philos->rules, philos->id);//mod
// }

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
	
	// fork_selection_process(philos, forks);
	fork_order(philos);//test
	//=> invert left and right to avoid weird deadlocks

	//left fork process
	if (check_death_flag(philos->monitor))
		return (false);//add
	// printf("philo %d is trying to take the left fork %d\n", philos->id, philos->l_fork->id);//debug
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
	//pourquoi locks mal faits? mauvaise attribution de fourchette?
	//philo pas encore parti mais unlock/destroy avant?
	//1&last rentrent deux fois dans release_forks
	//problème doit venir de là?
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
		return ;//leave	
	}
	pthread_mutex_lock(&philos->meal_lock);
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
	printf("philo %d left fork off\n", philos->id);
	pthread_mutex_unlock(&philos->r_fork->mutex);//same
	philos->has_right_fork = false;
	printf("philo %d right fork off\n", philos->id);
}

void	sleep_routine(t_philo *philos)
{
	if (check_death_flag(philos->monitor))
		return ;
	print_sleep(philos->monitor, philos->rules, philos->id);//mod
	smart_usleep(philos->rules->time_to_sleep, philos->rules->start_time, philos->monitor);//mod
	// if (check_death_flag(philos->monitor))
	// 	return ;//test
	// print_sleep(philos->monitor, philos->rules, philos->id);
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

/*
NOTES: def have to go back on all of this lol
for forks, if second fork block, wait
really have to do solo philo outside, as a specific edge case, uh
for think, maybe either small smart_usleep, or loop for the forks?
But forks need to have a check that you do it only if *both* are available
*/