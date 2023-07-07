# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdaly <jdaly@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/07 19:18:25 by jdaly             #+#    #+#              #
#    Updated: 2023/07/07 19:38:58 by jdaly            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread #-fsanitize=address -g

INC = philo.h

SRC = main.c

OBJ = $(SRC:.c=.o)

%.o: %.c $(INC)
        $(CC) -Wall -Wextra -Werror -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
        $(CC) $(CFLAGS) $(OBJ) -o $@

clean:
        rm -f $(OBJ)

fclean: clean
        rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re