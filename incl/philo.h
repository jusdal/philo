/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:27:52 by jdaly             #+#    #+#             */
/*   Updated: 2023/07/14 22:08:10 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define NC	"\e[0m"
# define YELLOW	"\e[1;33m"

typedef struct s_data
{
	int				total; //total number of philos
	time_t			die_time; //time to die
	time_t			eat_time; //time to eat
	time_t			sleep_time; //time to sleep
	time_t			must_eat; //number of times philo must eat
	time_t			start_time; //philo sim start time
	t_philo			**philos; //array of philos
	pthread_mutex_t	*forks;	
	pthread_t		monitor; //checks for any dead philos
	bool			stop; //sim is finished when true
}	t_data;

typedef struct s_philo
{
	int				num; //philo number
	pthread_t		tid; //thread id
	pthread_mutex_t	*fork_l; //right fork (ptr to left fork of adjacent philo)
	pthread_mutex_t	fork_r;
	t_data			*data; //pointer to data struct
	time_t			last_eaten; //time when philo last ate
	int				times_eaten; //number of times philo has eaten
	bool			finished; //true if eaten must_eat times
}	t_philo;

#endif