/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 12:38:28 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 12:49:46 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	save_start_time(t_table *table)
{
	int time_start;

	time_start = get_time(0);
	while (table->start == 0)
	{
		table->start = time_start;
		table = table->next;
	}
}

void	launch_threads(t_info info, t_table *table)
{
	save_start_time(table);
	while (table != NULL)
	{
		if (table->philo != NULL)
		{
			if (table->philo->nb % 2 == 0)
				pthread_create(&(table->philo->th), NULL, diner, (void *)table);
			if (table->philo->nb == info.nb_philosophers)
				break;
		}
		table = table->next;
	}
	while (table != NULL)
	{
		if (table->philo != NULL)
		{
			if (table->philo->nb % 2 != 0)
				pthread_create(&(table->philo->th), NULL, diner, (void *)table);
			if (table->philo->nb == 1)
				break;
		}
		table = table->prev;
	}
}

void	wait_threads(t_info info, t_table *table)
{
	int	i;
	
	i = 0;
	while (table != NULL && i < info.nb_philosophers)
	{
		if (table->philo != NULL)
		{
			pthread_join(table->philo->th, NULL);
			i++;
		}
		table = table->next;
	}
}
