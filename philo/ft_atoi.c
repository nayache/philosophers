/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 19:46:50 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 12:51:29 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_space(char c)
{
	if (c == '\n' || c == '\t')
		return (1);
	if (c == '\r' || c == '\f')
		return (1);
	if (c == ' ' || c == '\v')
		return (1);
	return (0);
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] != '\0' && is_space(str[i]) == 1)
		i++;
	while (str[i] != '\0' && is_digit(str[i]) == 1)
	{
		res = res * 10 + (str[i] - 48);
		i++;
	}
	return (res);
}
