/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:27:01 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/05 19:22:40 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	set_start_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_time(long start)
{
	struct timeval	current;
	long			now;

	gettimeofday(&current, NULL);
	now = (current.tv_sec * 1000) + (current.tv_usec / 1000);
	return (now - start);
}

void	smart_usleep(long duration, long start_time, t_monitor *monitor)
{
	long	current_start;

	current_start = get_time(start_time);
	while ((get_time(start_time) - current_start) < duration)
	{
		if (check_death_flag(monitor))
			break ;
		usleep(200);
	}
}

//temp
// void	colour_test_print()
// {
// 	printf("----------------\n");
// 	printf(CYAN "This is the cyan test\n" RES);
// 	printf("----------------\n");
// 	printf(MAGE "This is the magenta test\n" RES);
// 	printf("----------------\n");
// 	printf(GOLD "This is the golden test\n" RES);
// 	printf("----------------\n");
// 	printf(GREE "This is the greeen test\n" RES);
// 	printf("----------------\n");
// 	printf("Random " GREE "test. " RES "Thank you very much\n");
// 	printf("----------------\n");
// }//temp