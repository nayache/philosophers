/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 09:38:16 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 12:06:54 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	actions(t_table *table, t_philo *philo, long int start, int t_eat)
{
	if (check_death_on_the_table() == 1)
		return;
	if (time_limit_elapsed(table->info.ttd, philo->last_eaten, start) == 1)
	{
		print_status(table, philo->nb, start, 4);
		return;
	}
	print_status(table, table->philo->nb, start, 1);
	usleep(1000 * t_eat);
	lock_forks(philo->nb, &(table->next->mutex), &(table->prev->mutex));
	table->prev->fork = 1;
	table->next->fork = 1;
	unlock_forks(philo->nb, &(table->next->mutex), &(table->prev->mutex));
	philo->fork = 0;
	philo->nb_eaten += 1;
	if (table->info.max_eaten != philo->nb_eaten)
	{
		print_status(table, table->philo->nb, start, 2);
		usleep(1000 * table->info.tts);
		print_status(table, table->philo->nb, start, 3);
	}
}

void	take_forks(t_table *table, t_philo *philo, long int start)
{
	if (check_death_on_the_table() == 1)
		return;
	lock_forks(philo->nb, &(table->next->mutex), &(table->prev->mutex));
	if (table->next->fork == 1 && table->prev->fork == 1)
	{
		table->next->fork = 0;
		table->prev->fork = 0;
		unlock_forks(philo->nb, &(table->next->mutex), &(table->prev->mutex));
		philo->fork += 2;
		print_status(table, table->philo->nb, start, -1);
		print_status(table, table->philo->nb, start, 0);
	}
	else
		unlock_forks(philo->nb, &(table->next->mutex), &(table->prev->mutex));
}

void	take_fork(t_table *table, t_philo *philo, long int start)
{
	if (check_death_on_the_table() == 1 || philo->fork == 2)
		return;
	pthread_mutex_lock(&(table->next->mutex));
	if (table->next->fork == 1)
	{
		table->next->fork = 0;
		philo->fork += 1;
		pthread_mutex_unlock(&(table->next->mutex));
		print_status(table, table->philo->nb, start, 0);
	}
	else
		pthread_mutex_unlock(&(table->next->mutex));
	pthread_mutex_lock(&(table->prev->mutex));
	if (table->prev->fork == 1)
	{
		table->prev->fork = 0;
		philo->fork += 1;
		pthread_mutex_unlock(&(table->prev->mutex));
		print_status(table, table->philo->nb, start, -1);
	}
	else
		pthread_mutex_unlock(&(table->prev->mutex));
}

void	*diner(void *address)
{
	t_table			*table;
	t_philo			*philo;
	t_info			info;

	table = (t_table *)address;
	philo = table->philo;
	info = table->info;
	while (check_death_on_the_table() == 0)
	{
		if (info.max_eaten != -1 && table->philo->nb_eaten == info.max_eaten)
			return (NULL);
		if (time_limit_elapsed(info.ttd, philo->last_eaten, table->start) == 1)
		{
			print_status(table, table->philo->nb, table->start, 4);
			return (NULL);
		}
		if (table->philo->fork != 2)
			take_forks(table, table->philo, table->start);
		if (info.nb_philosophers % 2 != 0 && table->philo->fork != 2)
			take_fork(table, table->philo, table->start);
		if (table->philo->fork == 2)
			actions(table, table->philo, table->start, table->info.tte);
	}
	return (NULL);
}
