NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror -fsanitize=address
RM = rm -rf
LIBS = -lreadline
MKDIR = mkdir -p # Si ya existe no da problemas

INCLUDE_DIR = inc
INCLUDE_FLAG = -I$(INCLUDE_DIR)
INCLUDE = $(INCLUDE_DIR)/minishell.h

SRC_DIR = src/
SRC_FILES = main.c\
			signal/signal.c\
			libft/ft_putstr_fd.c\


SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR = obj/
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: $(OBJ_DIR) $(NAME)

# Crear el directorio de objetos
$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LIBS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INCLUDE)
	@$(MKDIR) $(dir $@)  #Crea el subdirectorio
	@$(CC) $(FLAGS) $(INCLUDE_FLAG) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

