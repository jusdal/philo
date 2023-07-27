/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:41:04 by jdaly             #+#    #+#             */
/*   Updated: 2023/07/28 01:47:55 by jdaly            ###   ########.fr       */
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

/* time functions */
time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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
	printf("%ld %d %s\n", get_time_ms() - philo->start_time, philo->num, status);
	pthread_mutex_unlock(&philo->data->print_lock);
}

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
		philos[i]->num = i + 1;
		philos[i]->tid = NULL;
		philos[i]->data = data;
		philos[i]->fork1 = i;
		philos[i]->fork2 = (i + 1) % data->total;
		philos[i]->last_eaten = get_time_ms();
		philos[i]->times_eaten = 0;
		philos[i]->finished = false;
		pthread_mutex_init(&philos[i]->mealtime_lock, NULL);
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
	return (data);
}

/* thread routine functions */
void	philo_thinking(t_philo *philo)
{
	write_status(philo, "is thinking");
	//printf("%ld: Philo %d is thinking\n", get_time_ms() - philo->start_time, philo->num);
	//usleep(50);
}

void	*philo_routine(void *data)
{
	t_philo *philo;
	philo = (t_philo *)data; //pthread_create takes void * argument so first I need to typecast
	philo->start_time = get_time_ms();
	philo->last_eaten = philo->start_time;
	//even: think first and then eat
	if (philo->num % 2 == 0)
	{
		philo_thinking(philo);
	}
	//odd: start eating, then sleep, then think
	while (philo->data->stop == false)
	{
		pthread_mutex_lock(&philo->data->forks[philo->fork1]);
		//write_status(philo, "has picked up fork 1");
		printf("%ld: Philo %d has picked up fork %d\n", get_time_ms() - philo->start_time, philo->num, philo->fork1);
		pthread_mutex_lock(&philo->data->forks[philo->fork2]);
		//write_status(philo, "has picked up fork 2");
		printf("%ld: Philo %d has picked up fork %d\n", get_time_ms() - philo->start_time, philo->num, philo->fork2);
		pthread_mutex_lock(&philo->mealtime_lock);
		philo->last_eaten = get_time_ms();
		pthread_mutex_unlock(&philo->mealtime_lock);
		write_status(philo, "is eating");
		//printf("%ld: Philo %d is eating\n", get_time_ms() - philo->start_time, philo->num);
  		usleep(philo->data->eat_time);
		philo->times_eaten += 1;
		pthread_mutex_unlock(&philo->data->forks[philo->fork1]);
		pthread_mutex_unlock(&philo->data->forks[philo->fork2]);
		write_status(philo, "is sleeping");
		//printf("%ld: Philo %d is sleeping\n", get_time_ms() - philo->start_time, philo->num);
		usleep(philo->data->sleep_time);
		philo_thinking(philo);
	}
	return (NULL);
}

/* monitor thread */

void	*monitor_routine(void *mdata)
{
	t_data	*data;
	int		i;

	data = (t_data *)mdata;
	while (1)
	{
		i = 0;
		while (i < data->total)
		{
			pthread_mutex_lock(&data->philos[i]->mealtime_lock);
			time_t last_eaten = data->philos[i]->last_eaten;
			pthread_mutex_unlock(&data->philos[i]->mealtime_lock);
			printf("Philo %d: time - last_eaten = %ld; die_time = %ld\n", data->philos[i]->num, (get_time_ms() - last_eaten), data->die_time);
			if ((get_time_ms() - last_eaten) >= data->die_time)
			{
				data->stop = true;
				//write_status(data->philos[i], "has died");
				printf("%s%ld %d has died\n%s", RED, get_time_ms() - data->philos[i]->start_time, data->philos[i]->num, RESET);
				return (NULL);
			}
			i++;
		}
		//usleep(100);
	}
}

void	start_threads(t_data *data)
{
	int i;
	i = 0;
	while (i < data->total)
	{
		pthread_create(&data->philos[i]->tid, NULL, &philo_routine, data->philos[i]);
		i += 2;
		if (i >= data->total && i % 2 == 0)
			i = 1;
	}
}

int	main(int ac, char *av[])
{
	t_data *data;
	int	i;

	if (ac < 5 || ac > 6)
		return (1);
	if ((!check_input(av)))
		error("Input Invalid\n", 2); //error number?
	data = init_data(ac, av);
	i = 0;

	start_threads(data);
	pthread_create(&data->monitor, NULL, &monitor_routine, data);
	pthread_join(data->monitor, NULL);
	//usleep(400000);
	return (0);
}