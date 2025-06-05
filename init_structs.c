/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:36:52 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/05 18:01:46 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_rules(t_rules *rules, char **av, int nb_philos)
{
	rules->total_philos = nb_philos;
	rules->time_to_die = ft_atol(av[2]);
	rules->time_to_eat = ft_atol(av[3]);
	rules->time_to_sleep = ft_atol(av[4]);
	if (av[5])
		rules->meals_required = ft_atol(av[5]);
	else
		rules->meals_required = 0;
	rules->start_time = set_start_time();
}

void	init_forks(t_fork *forks, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->total_philos)
	{
		forks[i].id = i;
		pthread_mutex_init(&forks[i].mutex, NULL);//no malloc to simplify the cleaning process
		i++;
	}
}

void	init_monitor(t_monitor *monitor, t_rules *rules, t_philo *philos)
{
	monitor->rules = rules;
	monitor->philos = philos;
	monitor->dead = 0;//for now, nobody's dead lol
	pthread_mutex_init(&monitor->death_lock, NULL);//no malloc, will access it by adress through philos
	pthread_mutex_init(&monitor->write_lock, NULL);//no malloc, will access it by adress through philos
}

void	init_philos(t_philo *philos, t_rules *rules, t_fork *forks, t_monitor *monitor)
{
	int	i;

	i = 0;//why not start at 1 directly? => because id - 1 is tricky
	while (i < rules->total_philos)
	{
		philos[i].id = i + 1;
		philos[i].rules = rules;
		philos[i].monitor = monitor;
		philos[i].meals_eaten = 0;//hasn't eaten just yet
		philos[i].last_meal = 0;//it was never, so 0;
		philos[i].l_fork = &forks[i];
		if (i == rules->total_philos - 1)
			philos[i].r_fork = &forks[0];
		else
			philos[i].r_fork = &forks[i + 1];
		philos[i].has_left_fork = false;//add
		philos[i].has_right_fork = false;//Add
		pthread_mutex_init(&philos[i].meal_lock, NULL);//no malloc: each their own
		philos[i].write_lock = &monitor->write_lock;//set in monitor
		philos[i].death_lock = &monitor->death_lock;//set in monitor
		i++;
	}
}

bool	alloc_init_all(t_data *data, char *av[], int nb_philos)
{
	data->rules = malloc(sizeof(t_rules));
	data->forks = malloc(sizeof(t_fork) * nb_philos);
	data->monitor = malloc(sizeof(t_monitor));
	data->philos = malloc(sizeof(t_philo) * nb_philos);
	if (!data->rules || !data->forks || !data->monitor || !data->philos)
		return(printf("malloc failed\n"), false);
	init_rules(data->rules, av, nb_philos);
	init_forks(data->forks, data->rules);
	init_monitor(data->monitor, data->rules, data->philos);
	init_philos(data->philos, data->rules, data->forks, data->monitor);
	return (true);
}
