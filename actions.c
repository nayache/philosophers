/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 09:38:16 by nayache           #+#    #+#             */
/*   Updated: 2021/08/06 07:45:38 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t writing;

int		get_time(long int start)
{
	struct timeval	current_time;

	if (start == 0)
	{
		gettimeofday(&current_time, NULL);
		start = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
		return (start);
	}
	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000 - start);
}

void	died(t_table *table)
{
	while (table->dead == 0)
	{
		table->dead = 1;
		table = table->next;
	}
}

void	write_status(t_table *table, int nb, int start, int status)
{
	pthread_mutex_lock(&writing);
	if (table->dead == 1)
	{
		pthread_mutex_unlock(&writing);
		return;
	}
	if (status == -1)
		printf("%dms %d has taken a left fork\n", get_time(start), nb);
	else if (status == 0)
		printf("%dms %d has taken a right fork\n", get_time(start), nb);
	else if (status == 1)
		printf("%dms \e[1;33m%d is eating\e[0;38m\n", get_time(start), nb);
	else if (status == 2)
		printf("%dms %d is sleeping\n", get_time(start), nb);
	else if (status == 3)
		printf("%dms %d is thinking\n", get_time(start), nb);
	else if (status == 4)
	{
		printf("%dms \e[1;31m%d id died\e[0;38m\n", get_time(start), nb);
		died(table);
	}
	pthread_mutex_unlock(&writing);
}

void	thinking(t_table *table, long int time_start)
{
	table->philo->slept = 0;
	table->philo->thought = 1;
	write_status(table, table->philo->nb, time_start, 3);
	usleep(10);
}

void	sleeping(t_table *table, long int time_start, int time_sleep)
{
	table->philo->eaten = 0;
	table->philo->slept = 1;
	write_status(table, table->philo->nb, time_start, 2);
	usleep(1000 * time_sleep);
}

void	eating(t_table *table, t_philo *philo, long int start, int t_eat)
{
	if (table->dead == 1)
		return;
	philo->last_eaten = get_time(start);
	write_status(table, table->philo->nb, start, 1);
	usleep(1000 * t_eat);
	table->prev->fork = 1;
	table->next->fork = 1;
	philo->fork = 0;
	table->philo->thought = 0;
	philo->eaten = 1;
	philo->nb_eaten += 1;
}

void	take_fork(t_table *table, t_philo *philo, long int start)
{
	if (table->dead == 1)
		return;
	if (get_time(table->start) - table->philo->last_eaten > table->info.time_die)
	{
		write_status(table, table->philo->nb, table->start, 4);
		return;
	}
	pthread_mutex_lock(&(table->prev->mutex));
	pthread_mutex_lock(&(table->next->mutex));
	if (table->prev->fork == 1 && table->next->fork == 1)
	{
		table->prev->fork = 0;
		table->next->fork = 0;
		philo->fork += 2;
		write_status(table, table->philo->nb, start, -1);
		write_status(table, table->philo->nb, start, 0);
	}
	pthread_mutex_unlock(&(table->next->mutex));
	pthread_mutex_unlock(&(table->prev->mutex));
}

void	*action(void *address)
{
	struct timeval	time;
	t_table			*table;
	t_info			info;

	table = (t_table *)address;
	info = table->info;
	while (table->dead == 0)
	{
		if (info.max_eaten != -1 && table->philo->nb_eaten == info.max_eaten)
			return (NULL);
		if (get_time(table->start) - table->philo->last_eaten > table->info.time_die)
		{
			write_status(table, table->philo->nb, table->start, 4);
			return (NULL);
		}
		if (table->philo->eaten == 1)
			sleeping(table, table->start, table->info.time_sleep);
		if (table->philo->slept == 1)
			thinking(table, table->start);
		if (table->philo->thought == 1)
		{
			while (table->philo->fork != 2 && table->dead == 0)
				take_fork(table, table->philo, table->start);
			if (table->philo->fork == 2)
				eating(table, table->philo, table->start, table->info.time_eat);
		}
	}
	return (NULL);
}
