/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 15:58:12 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/05 17:53:50 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_fork(t_monitor *monitor, t_rules *rules, int id)
{
	pthread_mutex_lock(&monitor->write_lock);
	printf("%zums ", get_time(rules->start_time));
	printf("%d has taken a fork\n", id);
	pthread_mutex_unlock(&monitor->write_lock);
}

void	print_eat(t_monitor *monitor, t_rules *rules, int id)
{
	pthread_mutex_lock(&monitor->write_lock);
	printf(GOLD"%zums ", get_time(rules->start_time));
	printf("%d is eating\n"RES, id);
	pthread_mutex_unlock(&monitor->write_lock);
}

void	print_sleep(t_monitor *monitor, t_rules *rules, int id)
{
	pthread_mutex_lock(&monitor->write_lock);
	printf(GREE"%zums ", get_time(rules->start_time));
	printf("%d is sleeping\n"RES, id);
	pthread_mutex_unlock(&monitor->write_lock);
}

void	print_think(t_monitor *monitor, t_rules *rules, int id)
{
	pthread_mutex_lock(&monitor->write_lock);
	printf(CYAN"%zums ", get_time(rules->start_time));
	printf("%d is thinking\n"RES, id);
	pthread_mutex_unlock(&monitor->write_lock);
}

void	print_dead(t_monitor *monitor, t_rules *rules, int id)
{
	pthread_mutex_lock(&monitor->death_lock);
	monitor->dead = 1;
	pthread_mutex_unlock(&monitor->death_lock);
	pthread_mutex_lock(&monitor->write_lock);
	printf(MAGE"%zums ", get_time(rules->start_time));
	printf("%d died\n"RES, id);
	pthread_mutex_unlock(&monitor->write_lock);
}
