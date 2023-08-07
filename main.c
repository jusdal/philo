/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justindaly <justindaly@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:41:04 by jdaly             #+#    #+#             */
/*   Updated: 2023/08/07 07:09:23 by justindaly       ###   ########.fr       */
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
		// if (philos[i]->num % 2)
		// {
		// 	philos[i]->fork1 = (i + 1) % data->total;
		// 	philos[i]->fork2 = i;
		// }
		philos[i]->last_eaten = get_time_ms();
		philos[i]->times_eaten = 0;
		philos[i]->eating = false;
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
	if ((time - philo->last_eaten) >= philo->data->die_time && philo->eating == false)
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

/* thread routine functions */

void	philo_sleeping(t_data *data, time_t sleep_time)
{
	time_t wake_up;

	wake_up = get_time_ms() + sleep_time;
	while (get_time_ms() < wake_up)
	{
		if (data->stop == true)
			break ;
		usleep(100);
	}
}

void	philo_thinking(t_philo *philo)
{
	time_t time_to_think;

	time_to_think = (philo->data->die_time - (get_time_ms() - philo->last_eaten) - philo->data->eat_time) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	//if (silent == false)
	write_status(philo, "is thinking");
	//usleep(60);
	philo_sleeping(philo->data, time_to_think);
	//printf("%ld: Philo %d is thinking\n", get_time_ms() - philo->start_time, philo->num);
	//usleep(100);
}


void	*philo_routine(void *data)
{
	t_philo *philo;
	philo = (t_philo *)data; //pthread_create takes void * argument so first I need to typecast
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_eaten = philo->data->start_time;
	pthread_mutex_unlock(&philo->meal_lock);
	//even: think first and then eat
	sim_start_delay(philo->data->start_time);
	if (philo->num % 2 == 0)
	{
		philo_thinking(philo);
	}
	//odd: start eating, then sleep, then think
	while (has_simulation_stopped(philo->data) == false)
	{
		pthread_mutex_lock(&philo->data->forks[philo->fork1]);
		write_status(philo, "has picked up fork 1");
		//printf("%ld: Philo %d has picked up fork %d\n", get_time_ms() - philo->start_time, philo->num, philo->fork1);
		pthread_mutex_lock(&philo->data->forks[philo->fork2]);
		write_status(philo, "has picked up fork 2");
		pthread_mutex_lock(&philo->meal_lock);
		philo->eating = true;
		philo->last_eaten = get_time_ms();
		pthread_mutex_unlock(&philo->meal_lock);
		write_status(philo, "is eating");
		//printf("%ld: Philo %d has picked up fork %d\n", get_time_ms() - philo->start_time, philo->num, philo->fork2);
		//printf("%ld: Philo %d is eating\n", get_time_ms() - philo->start_time, philo->num);
  		philo_sleeping(philo->data, philo->data->eat_time);
		if (has_simulation_stopped(philo->data) == false)
		{
			pthread_mutex_lock(&philo->meal_lock);
			philo->times_eaten += 1;
			philo->eating = false;
			pthread_mutex_unlock(&philo->meal_lock);
		}
		pthread_mutex_unlock(&philo->data->forks[philo->fork2]);
		pthread_mutex_unlock(&philo->data->forks[philo->fork1]);
		write_status(philo, "is sleeping");
		//printf("%ld: Philo %d is sleeping\n", get_time_ms() - philo->start_time, philo->num);
		philo_sleeping(philo->data, philo->data->sleep_time);
		philo_thinking(philo);
	}
	return (NULL);
}


bool	start_threads(t_data *data)
{
	int i;

	i = 0;
	data->start_time = get_time_ms() + (data->total * 2 * 10);
	while (i < data->total)
	{
		if (pthread_create(&data->philos[i]->tid, NULL, &philo_routine, data->philos[i]) != 0)
			return(free_error(data, "error: could not create thread\n", 0));
		i++;
	}
	if (data->total > 1)
	{
		if (pthread_create(&data->monitor, NULL, &monitor_routine, data) != 0)
			return (free_error(data, "error: could not create thread\n", 0));
	}
	return (true);
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
	t_data *data;
	//int	i = 0;

	if (ac < 5 || ac > 6)
		return (1);
	if ((!check_input(av)))
		error("Input Invalid\n", 2); //error number?
	data = init_data(ac, av);
	if (!data)
		return (EXIT_FAILURE);
	if (!start_threads(data))
		return (EXIT_FAILURE);
	stop_dining(data);
	return (EXIT_SUCCESS);
}