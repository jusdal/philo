/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 19:41:04 by jdaly             #+#    #+#             */
/*   Updated: 2023/07/11 21:02:01 by jdaly            ###   ########.fr       */
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
	int i;
	int j;
	
	i = 1;
	while (av[i])
	{
		j = -1;
		while(av[i][++j])
		{
			if (ft_isdigit(av[i][j]) == 1)
			{
				printf("NOT A DIGIT\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

/* function to initialize structs
void	init_philos(char *av[], t_data *data)
{
	//
}*/

int	main(int ac, char *av[])
{
	if (ac < 5 || ac > 6)
		return (1);
	if ((!check_input(av)))
		printf("Input Invalid\n");
	return (0);
}