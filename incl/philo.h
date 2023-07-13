/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:27:52 by jdaly             #+#    #+#             */
/*   Updated: 2023/07/13 19:04:26 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>


# define NC	"\e[0m"
# define YELLOW	"\e[1;33m"

typedef struct s_data
{
int			total; //total number of philos
int			die_time; //time to die
int 		eat_time; //time to eat
int			sleep_time; //time to sleep
int			must_eat; //number of times philo must eat
pthread_t	monitor; //checks for any dead philos

} t_data;

typedef struct s_philo
{
int	num; //philo number
pthread_t		tid; //thread id
pthread_mutex_t	*fork_l; //right fork (pointer to left fork of adjacent philo)
pthread_mutex_t fork_r;
t_data			*data; //pointer to data struct
bool			finish; //true if eaten must_eat times
} t_philo;

#endif