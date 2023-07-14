/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:41:04 by jdaly             #+#    #+#             */
/*   Updated: 2023/07/15 02:55:01 by jdaly            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/philo.h"

int	ft_isdigit(int c)
{
	if (!(c >= '0' && c <= '9'))
		return (0);
	else
		return (1);
}

//function to check argument input
int	check_input(char *av[])
{
	int	i;
	int	j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (!(ft_isdigit(av[i][j])))
				return (0);
		}
	}
	return (1);
}

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

/* functions to initialize structs */
t_data	*init_data(int ac, char *av[], t_data *data)
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
	return (data);
}

t_philo	**init_philos(char *av[], t_data *data)
{
	int		i;
	t_philo	**philos;

	i = 0;
	malloc(sizeof(t_philo) * data->total);
	while (i < data->total)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			free_error("Malloc Error\n", EXIT_FAILURE);
		philos[i]->num = i;
		philos[i]->data = data;
		philos[i]->times_eaten = 0;
		philos[i]->finished = false;
	}
	return (philos);
}

void	init_forks(t_data *data)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * data->total);
	i = 0;
	while (i < data->total)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	philo_routine(t_data *data)
{
	//even: think first and then eat
	if (i % 2 == 0)
	{
		usleep(data->eat_time);
	}
	//odd: start eating
	pthread_mutex_lock(&data->forks[]); //which fork?
  	usleep(data->eat_time);
    pthread_mutex_unlock(&data->forks[]); //which fork?
	//even: think first and then eat
	
}

/* time functions */
time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	main(int ac, char *av[])
{	
	//time_t start_time;
	//start_time = get_time_ms();
	//printf("START TIME: %ld\n", start_time);
	if (ac < 5 || ac > 6)
		return (1);
	if ((!check_input(av)))
		error("Input Invalid\n");
	//usleep(5000);
	//printf("ELAPSED TIME: %ld\n", get_time_ms() - start_time);
	return (0);
}