/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justindaly <justindaly@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 07:33:37 by justindaly        #+#    #+#             */
/*   Updated: 2023/08/07 20:41:21 by justindaly       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/philo.h"

/* functions to initialize structs */

int	ft_atoi(const char *str)
{
	int	holder;
	int	minuscounter;
	int	i;

	i = 0;
	minuscounter = 1;
	holder = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minuscounter = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		holder = holder * 10 + (str[i] - '0');
		i++;
	}
	return (holder * minuscounter);
}

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
		// if (philos[i]->num % 2)
		// {
		// 	philos[i]->fork1 = (i + 1) % data->total;
		// 	philos[i]->fork2 = i;
		// }
		philos[i]->last_eaten = get_time_ms();
		philos[i]->times_eaten = 0;
		pthread_mutex_init(&philos[i]->meal_lock, NULL);
		i++;
	}
	return (philos);
}

t_data	*init_data(int ac, char *av[])
{
	t_data *data;

	data = malloc(sizeof(t_data) * 1);
	data->total = ft_atoi(av[1]);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	data->must_eat = -1;
	if (ac - 1 == 5)
		data->must_eat = ft_atoi(av[5]);
	data->philos = init_philos(data);
	if(!data->philos)
	 	return (NULL);
	data->forks = init_forks(data);
	if (!data->forks)
		return (NULL);
	data->stop = false;
	pthread_mutex_init(&data->stop_lock, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	return (data);
}