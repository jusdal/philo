/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justindaly <justindaly@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 19:25:50 by justindaly        #+#    #+#             */
/*   Updated: 2023/08/07 19:28:35 by justindaly       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/philo.h"

/* time functions */
time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	sim_start_delay(time_t start_time)
{
	while (get_time_ms() < start_time)
		continue ;
}

/* write status */
void	write_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (philo->data->stop == true)
	{
		pthread_mutex_unlock(&philo->data->print_lock);
		return ;
	}
	printf("%ld %d %s\n", get_time_ms() - philo->data->start_time, (philo->num + 1), status);
	pthread_mutex_unlock(&philo->data->print_lock);
}