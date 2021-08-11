/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayache <nayache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 04:51:26 by nayache           #+#    #+#             */
/*   Updated: 2021/08/11 13:20:14 by nayache          ###   ########.fr       */
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

pthread_mutex_t	g_writing;
pthread_mutex_t	g_death;
int				g_dead;

typedef struct s_info
{
	int				nb_philosophers;
	int				tte;
	int				ttd;
	int				tts;
	int				max_eaten;
}					t_info;

typedef struct s_philo
{
	pthread_t		th;
	int				nb;
	int				fork;
	int				nb_eaten;
	int				last_eaten;
}					t_philo;

typedef struct s_table
{
	struct s_philo	*philo;
	struct s_info	info;
	struct s_table	*prev;
	struct s_table	*next;
	int				fork;
	long int		start;
	pthread_mutex_t	mutex;
}					t_table;

t_philo	*init_philo(int	nb);
t_table	*init_table(t_info info);
t_table	*add_fork(t_table *current, t_info info);
t_table	*build_table(t_info info);
t_table	*build_single_table(t_info info);
int		is_digit(char c);
void	free_table(t_table *head);
void	print_death(int time, int nb);
void	print_status(t_table *table, int nb, int start, int status);
void	launch_threads(t_info info, t_table *table);
void	wait_threads(t_info info, t_table *table);
int		ft_atoi(char *str);
void	*diner(void *address);
void	*action_even(void *address);
int		get_time(long int time_start);
int		time_limit_elapsed(int time_to_die, int time_last_eaten, int start);
void	lock_forks(int nb, void *next, void *prev);
void	unlock_forks(int nb, void *next, void *prev);
int		check_death_on_the_table(void);
int		get_time(long int start);
#endif
