/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 09:38:16 by nayache           #+#    #+#             */
/*   Updated: 2021/08/09 09:17:50 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t writing;

int		get_time(long int start)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
	if (start != 0)
		return (curr_time.tv_sec * 1000 + curr_time.tv_usec / 1000 - start);
	return (curr_time.tv_sec * 1000 + curr_time.tv_usec / 1000);
}

void	died(t_table *table)
{
	while (table->dead == 0)
	{
		table->dead = 1;
		table = table->next;
	}
}

void	write_status(t_philo *philo, int *dead, int nb, int start, int status)
{
	int	time;

	pthread_mutex_lock(&writing);
	if (*dead == 1)
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
		philo->last_eaten = time;
		printf("%dms \e[1;33m%d is eating\e[0;38m\n", time, nb);
	}
	else if (status == 2)
		printf("%dms %d is sleeping\n", time, nb);
	else if (status == 3)
		printf("%dms %d is thinking\n", time, nb);
	else if (status == 4)
		printf("%dms \e[1;31m%d id died\e[0;38m\n", time, nb);
	pthread_mutex_unlock(&writing);
}

void	thinking(t_table *table, long int start)
{
	//	table->philo->slept = 0;
	//	table->philo->thought = 1;
	write_status(table->philo, &(table->dead), table->philo->nb, start, 3);
}

void	sleeping(t_table *table, long int start, int time_sleep)
{
	//	table->philo->eaten = 0;
	//	table->philo->slept = 1;
	write_status(table->philo, &(table->dead), table->philo->nb, start, 2);
	usleep(1000 * time_sleep);
}

void	eating(t_table *table, t_philo *philo, long int start, int t_eat)
{
	if (table->dead == 1)
		return;
	//	philo->last_eaten = get_time(start);
	write_status(table->philo, &(table->dead), table->philo->nb, start, 1);
	usleep(1000 * t_eat);
	table->prev->fork = 1;
	table->next->fork = 1;
	philo->fork = 0;
	table->philo->thought = 0;
	//	philo->eaten = 1;
	philo->nb_eaten += 1;
	sleeping(table, start, table->info.time_sleep);
	thinking(table, start);
}

void	take_fork(t_table *table, t_philo *philo, long int start)
{
	if (table->dead == 1)
		return;
	pthread_mutex_lock(&(table->next->mutex));
	pthread_mutex_lock(&(table->prev->mutex));
	if (table->next->fork == 1 && table->prev->fork == 1)
	{
		table->next->fork = 0;
		table->prev->fork = 0;
		philo->fork += 2;
		pthread_mutex_unlock(&(table->next->mutex));
		pthread_mutex_unlock(&(table->prev->mutex));
		write_status(table->philo, &(table->dead), table->philo->nb, start, -1);
		write_status(table->philo, &(table->dead), table->philo->nb, start, 0);
	}
	else
	{
		pthread_mutex_unlock(&(table->next->mutex));
		pthread_mutex_unlock(&(table->prev->mutex));
	}
}

void	take_fork_even(t_table *table, t_philo *philo, long int start)
{
	if (table->dead == 1 || philo->fork == 2)
		return;
	pthread_mutex_lock(&(table->next->mutex));
	if (table->next->fork == 1)
	{
		table->next->fork = 0;
		philo->fork += 1;
		pthread_mutex_unlock(&(table->next->mutex));
		write_status(table->philo, &(table->dead), table->philo->nb, start, 0);
	}
	else
		pthread_mutex_unlock(&(table->next->mutex));
	pthread_mutex_lock(&(table->prev->mutex));
	if (table->prev->fork == 1)
	{
		table->prev->fork = 0;
		philo->fork += 1;
		pthread_mutex_unlock(&(table->prev->mutex));
		write_status(table->philo, &(table->dead), table->philo->nb, start, -1);
	}
	else
		pthread_mutex_unlock(&(table->prev->mutex));
}

void	take_fork_odd(t_table *table, t_philo *philo, long int start)
{
	if (table->dead == 1)
		return;
	if (table->next->fork == 1)
		take_fork(table, philo, start);
}

void	*action_even(void *address)
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
		if (get_time(table->start) - table->philo->last_eaten > info.time_die)
		{
			write_status(table->philo, &(table->dead), table->philo->nb, table->start, 4);
			died(table);
			return (NULL);
		}
		if (table->philo->fork != 2)
			take_fork(table, table->philo, table->start);
		if (table->philo->fork == 2)
			eating(table, table->philo, table->start, table->info.time_eat);
	}
	return (NULL);
}

void	*action(void *address)
{
	t_table			*table;
	t_info			info;

	table = (t_table *)address;
	info = table->info;
	while (table->dead == 0)
	{
		if (info.max_eaten != -1 && table->philo->nb_eaten == info.max_eaten)
			return (NULL);
		if (get_time(table->start) - table->philo->last_eaten > info.time_die)
		{
			write_status(table->philo, &(table->dead), table->philo->nb, table->start, 4);
			died(table);
			return (NULL);
		}
		if (table->philo->fork != 2)
			take_fork(table, table->philo, table->start);
		if (table->philo->fork != 2)
			take_fork_even(table, table->philo, table->start);
		if (table->philo->fork == 2)
			eating(table, table->philo, table->start, table->info.time_eat);
	}
	return (NULL);
}
