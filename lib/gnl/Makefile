# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 14:37:19 by emcorona          #+#    #+#              #
#    Updated: 2025/04/25 18:35:30 by emcorona         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# MAKE FILE GNL************************************************************** #
# 
#VAR---------------------------------------------------------------------------
NAME = libgnl.a
CC = cc
CFLAGS = -Wall -Werror -Wextra
SRC = get_next_line.c get_next_line_utils.c 
OBJ := $(SRC:%.c=%.o)

#RULES-------------------------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re