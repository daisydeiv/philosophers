/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:07:47 by mle-brie          #+#    #+#             */
/*   Updated: 2025/06/05 19:14:07 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
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
	int			total_philos;
	long		time_to_sleep;
	long		time_to_eat;
	long		time_to_die;
	long		meals_required;
	long		start_time;
}	t_rules;

//struct for the forks
typedef struct s_forks
{
	int				id;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_monitor	t_monitor;
//structure readability

//struct per philo
typedef struct s_philosophers
{
	pthread_t		p_thread;
	int				id;
	int				meals_eaten;
	long			last_meal;
	t_rules			*rules;
	t_fork			*r_fork;
	t_fork			*l_fork;
	bool			has_left_fork;
	bool			has_right_fork;

	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*death_lock;
	pthread_mutex_t	*write_lock;

	t_monitor		*monitor;
}	t_philo;

//monitoring structure
typedef struct s_monitor
{
	pthread_t			m_thread;
	t_philo				*philos;
	int					dead;
	t_rules				*rules;
	pthread_mutex_t		death_lock;
	pthread_mutex_t		write_lock;
}	t_monitor;

//centralized struct
typedef struct s_all_data
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
void	init_philos(t_philo *philos, t_rules *rules,
			t_fork *forks, t_monitor *monitor);
bool	alloc_init_all(t_data *data, char *av[], int nb_philos);

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
//utils
void	handle_one(t_data *data);
bool	check_death_flag(t_monitor *monitor);
void	check_meals(t_monitor *monitor, int i, int *full);
void	set_death_flag(t_monitor *monitor);
bool	die_and_release(t_philo *philos);//test
//philos' steps
bool	fork_routine(t_philo *philos, t_fork *forks);
void	eat_routine(t_philo *philos);
void	sleep_routine(t_philo *philos);
void	think_routine(t_philo *philos);
// simulation/process
void	*monitor_routine(void *arg);
void	*philo_routine(void *arg);
void	launch_simulation(t_data *data_array, int total_philos);
bool	set_data_array(t_data *data);

//cleanup
void	release_forks(t_philo *philos);
void	free_all(t_data *data);

// temp/print statement
void	colour_test_print(void);

#endif