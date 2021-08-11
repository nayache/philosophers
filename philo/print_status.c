/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 12:06:34 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 12:06:51 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_death(int time, int nb)
{
	pthread_mutex_lock(&death);
	dead = 1;
	pthread_mutex_unlock(&death);
	printf("%dms \e[1;31m%d id died\e[0;38m\n", time, nb);
}

void	print_status(t_table *table, int nb, int start, int status)
{
	int	time;

	pthread_mutex_lock(&writing);
	if (check_death_on_the_table() == 1)
	{
		pthread_mutex_unlock(&writing);
		return;
	}
	time = get_time(start);
	if (status == -1)
		printf("%dms %d has taken a left fork\n", time, nb);
	else if (status == 0)
		printf("%dms %d has taken a right fork\n", time, nb);
	else if (status == 1)
	{
		table->philo->last_eaten = time;
		printf("%dms \e[1;33m%d is eating\e[0;38m\n", time, nb);
	}
	else if (status == 2)
		printf("%dms %d is sleeping\n", time, nb);
	else if (status == 3)
		printf("%dms %d is thinking\n", time, nb);
	else
		print_death(time, nb);
	pthread_mutex_unlock(&writing);
}
