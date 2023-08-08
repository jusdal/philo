/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 23:08:29 by jdaly             #+#    #+#             */
/*   Updated: 2023/08/08 17:10:45 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/philo.h"

int	error(char *message, int exit_nbr)
{
	printf("%s\n", message);
	return (exit_nbr);
}

void	free_data(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->forks)
		free(data->forks);
	if (data->philos)
	{
		i = 0;
		while (i < data->total)
		{
			if (data->philos[i])
				free(data->philos[i]);
			i++;
		}
		free(data->philos);
	}
	free(data);
}

int	free_error(t_data *data, char *message, int exit_nbr)
{
	if (data != NULL)
		free_data(data);
	return (error(message, exit_nbr));
}

void	*null_error(t_data *data, char *message)
{
	if (data)
		free_data(data);
	error(message, EXIT_FAILURE);
	return (NULL);
}

void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->total)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i]->meal_lock);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->stop_lock);
}
