# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nayache <nayache@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/11 13:12:18 by nayache           #+#    #+#              #
#    Updated: 2021/08/11 13:16:45 by nayache          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS =	main.c build_table.c diner.c threads.c print_status.c utils.c utils2.c \
		ft_atoi.c

OBJS = $(SRCS:.c=.o)

$(NAME):	$(OBJS) $(LIBS)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lpthread
.c.o: 
		$(CC) $(CFLAGS) -c $< -o $@

all :	$(NAME)

clean : 
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

