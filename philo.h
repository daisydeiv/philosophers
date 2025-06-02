/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:07:47 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/02 20:21:04 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>
# include <limits.h>//for INT_MAX
# include <pthread.h>
// # include <time.h>
# include <sys/time.h>//gettimeofday()
// temp for prints
# define GREE	"\033[0;32m"//green colour
# define GOLD	"\033[0;33m"//gold colour
# define MAGE	"\033[0;35m"//magenta colour
# define CYAN	"\033[0;36m"//cyan colour
# define RES	"\033[0m"//put back to normal text
// use as such: printf(CYAN "test\n" RES);

//structures
//struct for the rules
typedef struct s_rules
{
	int			total_philos;//comp
	long		time_to_sleep;//time
	long		time_to_eat;//time
	long		time_to_die;//time
	long		meals_required;//number of meals needed
	long		start_time;//the start time stocked with get_time()
}	t_rules;

//struct for the forks
typedef struct s_forks
{
	int				id;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct 	s_monitor	t_monitor;
//structure readability

//struct per philo
typedef struct	s_philosophers
{
	pthread_t		p_thread;//philo's thread
	int				id;
	int				meals_eaten;//track number of meals
	long			last_meal;//time
	t_rules			*rules;
	t_fork			*r_fork;//right fork
	t_fork			*l_fork;//left fork

	pthread_mutex_t	meal_lock;//personnal lock
	pthread_mutex_t	*death_lock;//pointer to the shared lock
	pthread_mutex_t	*write_lock;//pointer to the shared lock

	t_monitor		*monitor;//supervises
}	t_philo;

//monitoring structure
typedef struct 	s_monitor
{
	pthread_t			m_thread;//monitor's thread
	t_philo				*philos;//an array, to loop other all of them with philos[i]
	int					dead;//flag, accessible by each philo, protected by mutex
	t_rules				*rules;
	pthread_mutex_t		death_lock;//has to check who is dead to quit the program
	pthread_mutex_t		write_lock;
}	t_monitor;

//centralized struct
typedef struct	s_all_data
{
	t_rules		*rules;
	t_fork		*forks;
	t_philo		*philos;
	t_monitor	*monitor;
}	t_data;


// input parsing
bool	is_num(char *str);
long	ft_atol(const char *str);
bool	parsing_args(int ac, char *av[], int *nb_philo);

//init structures
void	init_rules(t_rules *rules, char **av, int nb_philos);
void	init_forks(t_fork *forks, t_rules *rules);
void	init_monitor(t_monitor *monitor, t_rules *rules, t_philo *philos);
void	init_philos(t_philo *philos, t_rules *rules, t_fork *forks, t_monitor *monitor);
bool	alloc_init_all(t_data *data, char *av[], int nb_philos);

//utils
void	free_all(t_data *data);
//time
long	set_start_time(void);
long	get_time(long start);
void	smart_usleep(long duration, long start_time, t_monitor *monitor);
//print messages
void	print_fork(t_monitor *monitor, t_rules *rules, int id);
void	print_eat(t_monitor *monitor, t_rules *rules, int id);
void	print_sleep(t_monitor *monitor, t_rules *rules, int id);
void	print_think(t_monitor *monitor, t_rules *rules, int id);
void	print_dead(t_monitor *monitor, t_rules *rules, int id);

//simulation
void	handle_one(t_data *data);
bool	check_death_flag(t_monitor *monitor);
bool	set_data_array(t_data *data);
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);
void	launch_simulation(t_data *data_array, int total_philos);//other test
// simulation/process
// void	launch_simulation(t_philo *philos, t_monitor *monitor, t_rules *rules);//off for test
// void	launch_simulation(t_data *data);//test

// temp/print statement
void	colour_test_print();

#endif