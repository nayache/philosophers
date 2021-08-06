/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 04:51:26 by nayache           #+#    #+#             */
/*   Updated: 2021/08/06 07:27:49 by nayache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>

typedef	struct		s_info
{
	int				nb_philosophers;
	int				time_eat;
	int				time_die;
	int				time_sleep;
	int				max_eaten;
}					t_info;

typedef struct		s_philo
{
	pthread_t		th;
	int				nb;
	int				fork;
	int				eaten;
	int				slept;
	int				thought;
	int				nb_eaten;
	int				last_eaten;
}					t_philo;

typedef	struct		s_table
{
	struct s_philo	*philo;
	int				fork;
	int				dead;
	struct s_info	info;
	struct s_table	*prev;
	struct s_table	*next;
	long int		start;
	pthread_mutex_t	mutex;
}					t_table;


void	free_table(t_table *head);
t_philo	*init_philo(int	nb);
t_table	*init_table(t_info info);
t_table	*add_fork(t_table *current, t_info info);
t_table	*build_table(t_info info);
void	*action(void *address);
int		get_time(long int time_start);
#endif
