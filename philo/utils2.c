/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 11:55:20 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 12:09:15 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_forks(int nb, void *next, void *prev)
{
	if (nb % 2 == 0)
	{
		pthread_mutex_lock(next);
		pthread_mutex_lock(prev);
	}
	else
	{
		pthread_mutex_lock(prev);
		pthread_mutex_lock(next);
	}
}

void	unlock_forks(int nb, void *next, void *prev)
{
	if (nb % 2 == 0)
	{
		pthread_mutex_unlock(next);
		pthread_mutex_unlock(prev);
	}
	else
	{
		pthread_mutex_unlock(prev);
		pthread_mutex_unlock(next);
	}
}

int		check_death_on_the_table(void)
{
	pthread_mutex_lock(&death);
	if (dead == 1)
	{
		pthread_mutex_unlock(&death);
		return (1);
	}
	pthread_mutex_unlock(&death);
	return (0);
}

int		get_time(long int start)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
	if (start != 0)
		return (curr_time.tv_sec * 1000 + curr_time.tv_usec / 1000 - start);
	return (curr_time.tv_sec * 1000 + curr_time.tv_usec / 1000);
}

int		time_limit_elapsed(int time_to_die, int time_last_eaten, int start)
{
	if (get_time(start) - time_last_eaten >= time_to_die)
		return (1);
	return (0);
}
