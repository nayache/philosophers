/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 04:53:17 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 13:22:29 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_philo(int	nb)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (new == NULL)
		return (NULL);
	new->nb = nb;
	new->fork = 0;
	new->nb_eaten = 0;
	new->last_eaten = 0;
	return (new);
}

t_table	*init_table(t_info info)
{
	t_table	*new;

	new = malloc(sizeof(t_table));
	if (new == NULL)
		return (NULL);
	new->philo = NULL;
	new->start = 0;
	new->fork = -1;
	new->info = info;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

t_table	*add_fork(t_table *current, t_info info)
{
	t_table	*new;

	new = init_table(info);
	if (new == NULL)
		return (NULL);
	new->fork = 1;
	pthread_mutex_init(&(new->mutex), NULL);
	new->prev = current;
	new->next = NULL;
	current->next = new;
	return (new);
}

t_table	*build_single_table(t_info info)
{
	t_table	*head;
	t_table	*new;
	t_table	*fork;

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

t_table	*build_table(t_info info)
{
	t_table	*head;
	t_table	*current;
	t_table	*tmp;
	int		i;

	head = NULL;
	i = -1;
	while (++i < info.nb_philosophers)
	{
		current = init_table(info);
		if (current == NULL)
			return (NULL);
		current->philo = init_philo(i + 1);
		if (head == NULL)
			head = current;
		else
		{
			tmp->next = current;
			current->prev = tmp;
		}
		tmp = add_fork(current, info);
	}
	tmp->next = head;
	head->prev = tmp;
	return (head);
}
