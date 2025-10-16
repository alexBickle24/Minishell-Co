NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror 
RM = rm -rf
LIBS = -lreadline
MKDIR = mkdir -p 

INCLUDE_DIR = inc
LIBFT_DIR = inc/libft
LIBFT_A = $(LIBFT_DIR)/libft.a
INCLUDE_FLAG = -I$(INCLUDE_DIR) -I$(LIBFT_DIR)
INCLUDE = $(INCLUDE_DIR)/minishell.h

SRC_DIR = src/

SRC_FILES=  mains/main.c\
			builtins/builts_utils.c\
			builtins/ft_env.c\
			builtins/ft_cd.c\
			builtins/ft_cd2.c\
			builtins/ft_cd3.c\
			builtins/ft_cd4.c\
			builtins/ft_unset.c\
			builtins/ft_echo.c\
			builtins/ft_exit.c\
			builtins/ft_pwd.c\
			builtins/ft_export.c\
			builtins/ft_export2.c\
			debugin/print.c\
			minishell/init.c\
			minishell/close_minishell.c\
			minishell/environ.c\
			minishell/environ2.c\
			minishell/environ_utils.c\
			minishell/signal.c\
			minishell/signal_handlers.c\
			minishell/warnings.c\
			minishell/sys_home_path.c\
			minishell/sys_host_user.c\
			minishell/sys_ps1.c\
			minishell/sys_utils.c\
			executor/executor.c\
			executor/executor_utils.c\
			executor/exec_utils.c\
			executor/exec_utils2.c\
			executor/error_exit.c\
			executor/redirs_utils.c\
			executor/redirs_utils2.c\
			executor/free_and_close.c\
			executor/wait_pids.c\
			parsing/init_parsing.c\
			parsing/lexer_parser.c\
			parsing/list_utils.c\
			parsing/heredoc.c \
			parsing/heredoc_utils.c \
			parsing/parsing_utils.c \
			parsing/parsing_utils2.c\
			parsing/expand.c\
			parsing/expand_utils.c\
			parsing/parsing.c\
			parsing/parsing2.c\
			parsing/parsing_errors.c\
			parsing/jump_table.c\
			parsing/jump_table2.c\
			parsing/lexer_utils.c\
			utils/data_formats.c

	
SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR = obj/
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: $(LIBFT_A) $(OBJ_DIR) $(NAME)
	@echo "\033[0;32mMinishell compiled successfully!!\033[30"

message:
	@echo "\033[38;5;28m███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \033[0m"
	@echo "\033[38;5;28m████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \033[0m"
	@echo "\033[38;5;28m██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \033[0m"
	@echo "\033[38;5;28m██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \033[0m"
	@echo "\033[38;5;28m██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\033[0m"
	@echo "\033[38;5;28m╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\033[0m"
	@echo "\033[1;33m               by vicalons && alcarril\033[0m"
	@echo ""

loading_bar:
	@for i in $$(seq 1 65); do \
		printf "\033[38;5;226m▓\033[0m"; \
		sleep 0.03; \
	done; \
	printf " 🦜\n"

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(LIBFT_A):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(MAKE) --no-print-directory message
	@$(MAKE) --no-print-directory loading_bar
	@echo ""
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT_A) -o $(NAME) $(LIBS)
	
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

