/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 20:30:52 by nayache           #+#    #+#             */
/*   Updated: 2021/08/06 07:28:29 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	save_time_conditions(t_info	*info, char **args, int size)
{
	info->nb_philosophers = atoi(args[0]);
	info->time_die = atoi(args[1]);
	info->time_eat = atoi(args[2]);
	info->time_sleep = atoi(args[3]);
	if (size == 5)
		info->max_eaten = atoi(args[4]);
	else
		info->max_eaten = -1;
}

int	is_even(int nb)
{
	if (nb % 2 == 0)
		return (1);
	return (0);
}

void	save_start_time(t_table *table)
{
	int time_start;

	time_start = get_time(0);
	while (table->start == 0)
	{
		table->start = time_start;
		table = table->next;
	}
}

int	start(t_info info, t_table *table)
{
	t_table	*cur;
	int i;

	i = 0;
	cur = table;

	save_start_time(table);
	while (table != NULL && i < info.nb_philosophers)
	{
		if (table->philo != NULL)
		{
			if (table->philo->nb % 2 != 0)
				pthread_create(&(table->philo->th), NULL, action, (void *)table);
			i++;
		}
		table = table->next;
	}
	usleep(10);
	while (table != NULL && i > 0)
	{
		if (table->philo != NULL)
		{
			if (table->philo->nb % 2 == 0)
				pthread_create(&(table->philo->th), NULL, action, (void *)table);
			i--;
		}
		table = table->prev;
	}

	i = 0;
	while (cur != NULL && i < info.nb_philosophers)
	{
		if (cur->philo != NULL)
		{
			pthread_join(cur->philo->th, NULL);
			printf("\e[1;32m%d -> %dtimes eaten\e[0;38m\n", cur->philo->nb, cur->philo->nb_eaten);
			i++;
		}
		cur = cur->next;
	}
	return (0);
}

t_table	*build_single_table(t_info info)
{
	t_table *head;
	t_table *new;
	t_table *fork;

	head = init_table(info);
	if (head == NULL)
		return (NULL);
	head->philo = init_philo(1);
	if (head->philo == NULL)
		return (NULL);
	fork = add_fork(head, info);
	new = init_table(info);
	pthread_mutex_init(&(new->mutex), NULL);
	new->next = head;
	head->prev = new;
	fork->next = new;
	return (head);
}

int	main(int ac, char **av)
{
	t_info	info;
	t_table	*table;

	if (ac < 5 || ac > 6)
		return (0);
	save_time_conditions(&info, av + 1, ac - 1);
	if (info.nb_philosophers < 1)
		return (-1);
	if (info.nb_philosophers == 1)
		table = build_single_table(info);
	else
		table = build_table(info);
	if (table == NULL)
		return (-1);
	start(info, table);
	free_table(table);
	return (0);
}
