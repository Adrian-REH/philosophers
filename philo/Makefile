# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/20 12:20:24 by adherrer          #+#    #+#              #
#    Updated: 2024/08/18 05:40:13 by adherrer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc

CFLAGS = -Werror -Wall -Wextra -g

RM = rm -rf


SRCS = 	src/philo.c src/time_utils.c src/handler_logs.c src/resource_utils.c
		
OBJ =  $(SRCS:.c=.o)


CYAN = \033[0;96m
DEF_COLOR = \033[0;49m

$(NAME) : $(OBJ)
	@echo "$(CYAN) ☕ EXECUTE DEFAULT PART! ☕ $(DEF_COLOR)"
	gcc $(CFLAGS)  $(OBJ) -o $(NAME)
	@echo "$(CYAN) ✨ ¡SUCCESS! ✨ $(DEF_COLOR)"


all : $(NAME)

fclean : clean
	$(RM) $(NAME)
	@echo "$(CYAN) ✨ ¡CLEANED! ✨ $(DEF_COLOR)"

clean :
	@echo "$(CYAN) 🍩 ¡INIT CLEAN! 🍩 $(DEF_COLOR)"
	$(RM) $(NAME)
	$(RM) $(OBJ)

re : fclean all


.PHONY:     all clean fclean re