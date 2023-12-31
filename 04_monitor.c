/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justindaly <justindaly@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 07:37:46 by justindaly        #+#    #+#             */
/*   Updated: 2023/08/07 20:41:00 by justindaly       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/philo.h"

/* monitor thread */
void	set_stop_flag(t_data *data, bool state)
{
	pthread_mutex_lock(&data->stop_lock);
	data->stop = state;
	pthread_mutex_unlock(&data->stop_lock);
}

bool	has_simulation_stopped(t_data *data)
{
	bool stopped;

	stopped = false;
	pthread_mutex_lock(&data->stop_lock);
	if (data->stop == true)
		stopped = true;
	pthread_mutex_unlock(&data->stop_lock);
	return (stopped);
}

bool	kill_philo(t_philo *philo)
{
	time_t time;

	time = get_time_ms();
	if ((time - philo->last_eaten) >= philo->data->die_time)
	{
		write_status(philo, "has died");
		set_stop_flag(philo->data, true);
		pthread_mutex_unlock(&philo->meal_lock);
		return (true);
	}
	return (false);
}

bool	end_condition_reached(t_data *data)
{
	int	i;
	bool all_eaten;

	all_eaten = true;
	i = 0;
	while (i < data->total)
	{
		pthread_mutex_lock(&data->philos[i]->meal_lock);
		if (kill_philo(data->philos[i]))
			return (true);
		if (data->must_eat != -1)
			if (data->philos[i]->times_eaten < data->must_eat)
				all_eaten = false;
		pthread_mutex_unlock(&data->philos[i]->meal_lock);
		i++;
	}
	if (data->must_eat != -1 && all_eaten == true)
	{
		set_stop_flag(data, true);
		return(true);
	}
	return (false);
}

void	*monitor_routine(void *mdata)
{
	t_data	*data;

	data = (t_data *)mdata;
	if (data->must_eat == 0)
		return NULL;
	set_stop_flag(data, false);
	sim_start_delay(data->start_time);
	while (1)
	{
		if (end_condition_reached(data) == true)
			return NULL;
		usleep(100);
	}
}