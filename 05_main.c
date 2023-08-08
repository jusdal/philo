/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:41:04 by jdaly             #+#    #+#             */
/*   Updated: 2023/08/08 17:10:00 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/philo.h"

bool	start_threads(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_time_ms() + (data->total * 2 * 10);
	while (i < data->total)
	{
		if (pthread_create(&data->philos[i]->tid, NULL, &philo_routine,
				data->philos[i]) != 0)
			return (free_error(data, "error: could not create thread\n", 0));
		i++;
	}
	if (data->total > 1)
	{
		if (pthread_create(&data->monitor, NULL, &monitor_routine, data) != 0)
			return (free_error(data, "error: could not create thread\n", 0));
	}
	return (true);
}

void	stop_dining(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->total)
	{
		pthread_join(data->philos[i]->tid, NULL);
		i++;
	}
	if (data->total > 1)
		pthread_join(data->monitor, NULL);
	destroy_mutexes(data);
	free_data(data);
}

int	main(int ac, char *av[])
{
	t_data	*data;

	if (ac < 5 || ac > 6)
		return (1);
	if ((!check_input(av)))
	{
		error("Input Invalid\n", 2);
		return (EXIT_FAILURE);
	}
	data = init_data(ac, av);
	if (!data)
		return (EXIT_FAILURE);
	if (!start_threads(data))
		return (EXIT_FAILURE);
	stop_dining(data);
	return (EXIT_SUCCESS);
}
