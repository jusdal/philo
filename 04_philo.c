/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 07:36:15 by justindaly        #+#    #+#             */
/*   Updated: 2023/08/08 22:23:22 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/philo.h"

/* philo thread routine functions */
static void	philo_sleeping(t_data *data, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time_ms() + sleep_time;
	while (get_time_ms() < wake_up)
	{
		if (data->stop == true)
			break ;
		usleep(100);
	}
}

static void	philo_thinking(t_philo *philo)
{
	time_t	time_to_think;

	time_to_think = (philo->data->die_time - (get_time_ms() - philo->last_eaten)
			- philo->data->eat_time) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	write_status(philo, "is thinking");
	philo_sleeping(philo->data, time_to_think);
}

static void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->fork1]);
	write_status(philo, "picked up fork 1");
	philo_sleeping(philo->data, philo->data->die_time);
	write_status(philo, "has died");
	pthread_mutex_unlock(&philo->data->forks[philo->fork1]);
	return (NULL);
}

static void	philo_loop(t_philo *philo)
{
	while (has_simulation_stopped(philo->data) == false)
	{
		pthread_mutex_lock(&philo->data->forks[philo->fork1]);
		write_status(philo, "has picked up fork 1");
		pthread_mutex_lock(&philo->data->forks[philo->fork2]);
		write_status(philo, "has picked up fork 2");
		pthread_mutex_lock(&philo->meal_lock);
		philo->last_eaten = get_time_ms();
		pthread_mutex_unlock(&philo->meal_lock);
		write_status(philo, "is eating");
		philo_sleeping(philo->data, philo->data->eat_time);
		if (has_simulation_stopped(philo->data) == false)
		{
			pthread_mutex_lock(&philo->meal_lock);
			philo->times_eaten += 1;
			pthread_mutex_unlock(&philo->meal_lock);
		}
		pthread_mutex_unlock(&philo->data->forks[philo->fork2]);
		pthread_mutex_unlock(&philo->data->forks[philo->fork1]);
		write_status(philo, "is sleeping");
		philo_sleeping(philo->data, philo->data->sleep_time);
		philo_thinking(philo);
	}
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_eaten = philo->data->start_time;
	pthread_mutex_unlock(&philo->meal_lock);
	sim_start_delay(philo->data->start_time);
	if (philo->data->total == 1)
		return (one_philo_routine(philo));
	else if (philo->num % 2 == 1)
		philo_thinking(philo);
	philo_loop(philo);
	return (NULL);
}
