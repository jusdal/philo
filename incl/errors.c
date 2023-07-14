/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 23:08:29 by jdaly             #+#    #+#             */
/*   Updated: 2023/07/15 02:06:14 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error(char *message, int exit_nbr)
{
	printf(message);
	printf("\n");
	return (exit_nbr);
}

void	free_data(t_data *data)
{
	int	i;

	if (!data)
		return (NULL);
	if (data->forks)
		free(data->forks);
	if(data->philos)
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
	return (NULL);
}

int	free_error(t_data *data, char *message, int exit_nbr)
{
	free_data(data);
	error(message, exit_nbr);
}
