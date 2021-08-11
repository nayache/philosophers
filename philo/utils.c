/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 11:38:56 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 13:20:48 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	free_table(t_table *head)
{
	t_table	*tmp;

	if (head->prev != NULL)
		head->prev->next = NULL;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		if (tmp->philo != NULL)
			free(tmp->philo);
		free(tmp);
		tmp = NULL;
	}
}
