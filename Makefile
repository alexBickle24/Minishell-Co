NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
RM = rm -rf
LIBS = -lreadline
MKDIR = mkdir -p # Si ya existe no da problemas

INCLUDE_DIR = inc
LIBFT_DIR = inc/libft
LIBFT_A = $(LIBFT_DIR)/libft.a
INCLUDE_FLAG = -I$(INCLUDE_DIR) -I$(LIBFT_DIR)
INCLUDE = $(INCLUDE_DIR)/minishell.h

SRC_DIR = src/
#falta añadir los builtins
#Cambiando el archivo de la carpeta main testeamos unas cosas u otras /mains/"archivo.c"
SRC_FILES=  mains/main_exec.c\
			debugin/print.c\
			minishell/init.c\
			minishell/close_minishell.c\
            minishell/environ.c\
			minishell/environ_utils.c\
            minishell/signal.c\
			minishell/warnings.c\
			executor/executor.c\
			executor/exec_utils.c\
			executor/exec_utils2.c\
			executor/error_exit.c\
			executor/redirs_utils.c\
			executor/free_and_close.c\
			executor/wait_pids.c\
			parsing/init_parsing.c\
			parsing/list_utils.c\
			parsing/heredoc.c \
			parsing/heredoc_utils.c \
			parsing/parsing_utils.c \
			utils/data_formats.c

	
SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR = obj/
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: $(LIBFT_A) $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(LIBFT_A):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT_A) -o $(NAME) $(LIBS)
	@echo "\033[0;32mMinishell compiled successfully!!\033[30"
	@echo "\033[0;32m███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \033[0m"
	@echo "\033[0;32m████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \033[0m"
	@echo "\033[0;32m██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \033[0m"
	@echo "\033[0;32m██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \033[0m"
	@echo "\033[0;32m██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\033[0m"
	@echo "\033[0;32m╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\033[0m"
	
$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INCLUDE)
	@$(MKDIR) $(dir $@)
	@$(CC) $(FLAGS) $(INCLUDE_FLAG) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) --no-print-directory clean -C $(LIBFT_DIR)

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) --no-print-directory fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re

