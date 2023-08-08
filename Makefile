# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: justindaly <justindaly@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/07 19:18:25 by jdaly             #+#    #+#              #
#    Updated: 2023/08/07 19:26:08 by justindaly       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread #-fsanitize=thread -g

INC = -I ./incl

SRC = 00_utils.c 01_checkargs.c 02_init.c 03_philo.c 04_monitor.c 05_main.c errors.c
OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) -Wall -Wextra -Werror -c $< -o $@ $(INC)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re