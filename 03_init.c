/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 07:33:37 by justindaly        #+#    #+#             */
/*   Updated: 2023/08/09 01:19:08 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/philo.h"

/* functions to initialize structs */
pthread_mutex_t	*init_forks(t_data *data)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * data->total);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < data->total)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
}

t_philo	**init_philos(t_data *data)
{
	int		i;
	t_philo	**philos;

	i = 0;
	philos = malloc(sizeof(t_philo) * data->total);
	if (!philos)
		return (null_error(data, "Malloc Error"));
	while (i < data->total)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (null_error(data, "Malloc Error"));
		philos[i]->num = i;
		philos[i]->tid = NULL;
		philos[i]->data = data;
		philos[i]->fork1 = i;
		philos[i]->fork2 = (i + 1) % data->total;
		philos[i]->last_eaten = get_time_ms();
		philos[i]->times_eaten = 0;
		pthread_mutex_init(&philos[i]->meal_lock, NULL);
		i++;
	}
	return (philos);
}

t_data	*init_data(int ac, char *av[])
{
	t_data	*data;

	data = malloc(sizeof(t_data) * 1);
	data->total = ft_pos_atoi(av[1]);
	data->die_time = ft_pos_atoi(av[2]);
	data->eat_time = ft_pos_atoi(av[3]);
	data->sleep_time = ft_pos_atoi(av[4]);
	data->must_eat = -1;
	if (ac - 1 == 5)
		data->must_eat = ft_pos_atoi(av[5]);
	data->philos = init_philos(data);
	if (!data->philos)
		return (NULL);
	data->forks = init_forks(data);
	if (!data->forks)
		return (NULL);
	if (pthread_mutex_init(&data->stop_lock, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (NULL);
	data->stop = false;
	return (data);
}
