# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/20 12:20:24 by adherrer          #+#    #+#              #
#    Updated: 2024/08/02 17:55:25 by adherrer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME_B = philo_bonus

CC = gcc

CFLAGS = -Werror -Wall -Wextra

RM = rm -rf		

SRCS_BONUS = 	src/philo_bonus.c



OBJ_B =  $(SRCS_BONUS:.c=.o)

CYAN = \033[0;96m
DEF_COLOR = \033[0;49m


$(NAME_B) : $(OBJ_B)
	@echo "$(CYAN) 🦾 EXECUTE BONUS! 🦾 $(DEF_COLOR)"
	gcc $(CFLAGS) $(OBJ_B)  -o $(NAME_B)
	@echo "$(CYAN) ✨ ¡SUCCESS! ✨ $(DEF_COLOR)"

all : $(NAME_B)

fclean : clean
	$(RM) $(NAME_B)
	@echo "$(CYAN) ✨ ¡CLEANED! ✨ $(DEF_COLOR)"

clean :
	@echo "$(CYAN) 🍩 ¡INIT CLEAN! 🍩 $(DEF_COLOR)"
	$(RM) $(OBJ_B)
	@echo "$(CYAN) ✨ ¡CLEANED! ✨ $(DEF_COLOR)"

re : fclean all

bonus : $(NAME_B)

.PHONY:     all clean fclean re