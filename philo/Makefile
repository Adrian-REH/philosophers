# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/20 12:20:24 by adherrer          #+#    #+#              #
#    Updated: 2024/08/17 16:53:33 by adherrer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc

CFLAGS = -Werror -Wall -Wextra -g

RM = rm -rf


SRCS = 	src/philo.c src/time_util.c
		
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
	@echo "$(CYAN) ✨ ¡CLEANED! ✨ $(DEF_COLOR)"

re : fclean all


.PHONY:     all clean fclean re