/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:21:55 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/03 11:47:59 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	temp_rules_print(t_rules *rules, t_fork *forks)
{
	printf("philo's informations:\n");
	printf("total number of philosophers: %d\n", rules->total_philos);
	printf("Total number of forks: %d\n", rules->total_philos);
	printf("Last fork ID: %d\n", forks[rules->total_philos - 1].id);//ID starts at 0
	printf(">fork check: %d\n", forks[rules->total_philos].id);//should print 0 since out of bond
	printf("time it takes to die = %ld\n", rules->time_to_die);
	printf("time it takes to eat = %ld\n", rules->time_to_eat);
	printf("time it takes to sleep = %ld\n", rules->time_to_sleep);
	printf("number of meals = %ld\n", rules->meals_required);
	printf("#############################\n");
}

int	main(int ac, char *av[])
{
	int			nb_philos;
	t_data		*data;

	nb_philos = 0;
	if (!parsing_args(ac, av, &nb_philos))
		return (1);//if it returned 1, so if it's an error

	data = malloc(sizeof(t_data));
	if (!data)
		return(printf("malloc failed\n"), 1);
	if (!alloc_init_all(data, av, nb_philos))
		return (1);

	temp_rules_print(data->rules, data->forks);//temp
	if (nb_philos == 1)
		handle_one(data);
	else
		if (!set_data_array(data))//other test
			return (1);//failed by malloc
		// printf(MAGE"Work In Progress\n"RES);//debug
	free_all(data);
	// colour_test_print();
	return (0);
}

/*
NOTES: 
since I use the t_data inside the main, when calling another struct, I have to pass by data->
but, all the functions I create will use the other structs, no worries, babe
*/