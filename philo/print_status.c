/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 12:06:34 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 13:25:51 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_death(int time, int nb)
{
	pthread_mutex_lock(&g_death);
	g_dead = 1;
	pthread_mutex_unlock(&g_death);
	printf("%dms %d id died\n", time, nb);
}

void	print_status(t_table *table, int nb, int start, int status)
{
	int	time;

	pthread_mutex_lock(&g_writing);
	if (check_death_on_the_table() == 1)
	{
		pthread_mutex_unlock(&g_writing);
		return ;
	}
	time = get_time(start);
	if (status == 0)
		printf("%dms %d has taken a fork\n", time, nb);
	else if (status == 1)
	{
		table->philo->last_eaten = time;
		printf("%dms %d is eating\n", time, nb);
	}
	else if (status == 2)
		printf("%dms %d is sleeping\n", time, nb);
	else if (status == 3)
		printf("%dms %d is thinking\n", time, nb);
	else
		print_death(time, nb);
	pthread_mutex_unlock(&g_writing);
}
