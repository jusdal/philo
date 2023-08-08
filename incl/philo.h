/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:27:52 by jdaly             #+#    #+#             */
/*   Updated: 2023/08/08 19:20:46 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "colors.h"
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				total;
	time_t			die_time;
	time_t			eat_time;
	time_t			sleep_time;
	time_t			must_eat;
	time_t			start_time;
	t_philo			**philos;
	pthread_mutex_t	*forks;
	pthread_t		monitor;
	pthread_mutex_t	stop_lock;
	bool			stop;
	pthread_mutex_t	print_lock;
}	t_data;

typedef struct s_philo
{
	int				num;
	pthread_t		tid;
	t_data			*data;
	int				fork1;
	int				fork2;
	time_t			last_eaten;
	int				times_eaten;
	pthread_mutex_t	meal_lock;
}	t_philo;

/* errors */
int		error(char *message, int exit_nbr);
void	free_data(t_data *data);
int		free_error(t_data *data, char *message, int exit_nbr);
void	*null_error(t_data *data, char *message);
void	destroy_mutexes(t_data *data);

/* utils */
time_t	get_time_ms(void);
void	sim_start_delay(time_t start_time);
void	write_status(t_philo *philo, char *status);

/* checkargs */
int		check_input(char *av[]);

/* init */
t_data	*init_data(int ac, char *av[]);

/* philo */
void	*philo_routine(void *data);

/* monitor */
void	*monitor_routine(void *mdata);
bool	has_simulation_stopped(t_data *data);

#endif