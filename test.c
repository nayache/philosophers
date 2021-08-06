/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 18:14:08 by nayache           #+#    #+#             */
/*   Updated: 2021/07/29 20:30:27 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int stock = 20;
static int clients = 0;
pthread_mutex_t	mutex;

void	*client(void *arg)
{
	clients++;
	pthread_mutex_lock(&mutex);
	if (stock > 4)
	{
		stock -= 5;
		printf("\e[1;33mVente 5 articles, reste %d\e[0;38m\n", stock);
	}
	else
	{
		printf("\e[1;31mStock epuise\e[0;38m\n");
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
	return (NULL);
}

void	*store(void *arg)
{
	while (clients < 1024)
	{
		usleep(100);
		pthread_mutex_lock(&mutex);
		if (stock < 20)
		{
			printf("\e[1;32mAjout de %d articles \e[0;38mStock = 20\n", 20 - stock);
			stock = 20;
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
	return (NULL);
}

int	main(int ac, char **av)
{
	int	i = 0;
	pthread_t	cli[1024];
	pthread_t	t2;

	pthread_mutex_init(&mutex, NULL);
	pthread_create(&t2, NULL, store, NULL);
	while (i < 1024)
	{
		pthread_create(&cli[i], NULL, client, NULL);
		i++;
	}
	for (int j = 0; j < 1024; j++)
		pthread_join(cli[j], NULL);
	pthread_join(t2, NULL);
	printf("END\n");
	return (0);
}
