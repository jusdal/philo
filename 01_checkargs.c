/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_checkargs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justindaly <justindaly@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 07:31:41 by justindaly        #+#    #+#             */
/*   Updated: 2023/08/07 21:10:02 by justindaly       ###   ########.fr       */
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
