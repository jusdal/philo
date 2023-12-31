/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justindaly <justindaly@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:27:52 by jdaly             #+#    #+#             */
/*   Updated: 2023/08/07 19:42:07 by justindaly       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "colors.h"
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo t_philo;

typedef struct s_data
{
	int				total; //total number of philos
	time_t			die_time; //time to die
	time_t			eat_time; //time to eat
	time_t			sleep_time; //time to sleep
	time_t			must_eat; //number of times philo must eat
	time_t			start_time; //philo simulation start time
	t_philo			**philos; //array of philos
	pthread_mutex_t	*forks;	
	pthread_t		monitor; //checks for any dead philos
	pthread_mutex_t	stop_lock; //prevents conflicts if multiple threads check stop bool
	bool			stop; //sim is finished when true
	pthread_mutex_t	print_lock; //to prevent printing after philo dies
}	t_data;

typedef struct s_philo
{
	int				num; //philo number
	pthread_t		tid; //thread id
	t_data			*data; //pointer to data struct
	int				fork1;
	int				fork2;
	//time_t			start_time; when philo started
	time_t			last_eaten; //time when philo last ate
	int				times_eaten; //number of times philo has eaten
	bool			eating;
	pthread_mutex_t	meal_lock;
}	t_philo;

int		error(char *message, int exit_nbr);
void	free_data(t_data *data);
int		free_error(t_data *data, char *message, int exit_nbr);
void	*null_error(t_data *data, char *message);
void	destroy_mutexes(t_data *data);

/* utils */
time_t	get_time_ms(void);
void	sim_start_delay(time_t start_time);
void	write_status(t_philo *philo, char *status);

int	check_input(char *av[]);

/* init */
t_data	*init_data(int ac, char *av[]);

void	*philo_routine(void *data);

void	*monitor_routine(void *mdata);

bool	has_simulation_stopped(t_data *data);



#endif