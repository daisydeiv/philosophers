/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:22:25 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/01 16:19:06 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	colour_test_print()
{
	printf("----------------\n");
	printf(CYAN "This is the cyan test\n" RES);
	printf("----------------\n");
	printf(MAGE "This is the magenta test\n" RES);
	printf("----------------\n");
	printf(GOLD "This is the golden test\n" RES);
	printf("----------------\n");
	printf(GREE "This is the greeen test\n" RES);
	printf("----------------\n");
	printf("Random " GREE "test. " RES "Thank you very much\n");
	printf("----------------\n");
}

/*
I think I can make a function that deals with the print statements?
like print_stat(start_time, "message") - but it should include the %d type of shit
or something more like print_dead(start, id), but for each action?
coz I need to include the write mutex (monitor, mostly), 
the time (rules), and the id (philos, mostly)
*/