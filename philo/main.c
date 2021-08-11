/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 20:30:52 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 12:50:12 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_args(char **args, int size)
{
	int			i;
	long int	nb;	
	
	while (--size >= 0)
	{
		i = 0;
		nb = 0;
		while (args[size][i] != '\0')
		{
			if (nb > 2147483647)
				return (-1);
			if (is_digit(args[size][i]) == 0)
				return (-1);
			nb = nb * 10 + args[size][i];
			i++;
		}
		size--;
	}
	return (0);
}

static int	save_time_conditions(t_info	*info, char **args, int size)
{
	if (check_args(args, size) == -1)
		return (-1);
	info->nb_philosophers = atoi(args[0]);
	info->ttd = ft_atoi(args[1]);
	info->tte = ft_atoi(args[2]);
	info->tts = ft_atoi(args[3]);
	if (size == 5)
		info->max_eaten = atoi(args[4]);
	else
		info->max_eaten = -1;
	return (0);
}

int	main(int ac, char **av)
{
	t_info	info;
	t_table	*table;

	if (ac < 5 || ac > 6)
		return (0);
	if (save_time_conditions(&info, av + 1, ac - 1) == -1)
	{
		printf("Error\nBad arguments\n");
		return (-1);
	}
	if (info.nb_philosophers < 1)
		return (-1);
	if (info.nb_philosophers == 1)
		table = build_single_table(info);
	else
		table = build_table(info);
	if (table == NULL)
		return (-1);
	dead = 0;
	pthread_mutex_init(&death, NULL);
	launch_threads(info, table);
	wait_threads(info, table);
	free_table(table);
	return (0);
}
