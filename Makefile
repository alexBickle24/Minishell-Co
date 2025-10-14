NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror
RM = rm -rf
LIBS = -lreadline
MKDIR = mkdir -p 

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a
INCLUDE_FLAG = -I$(LIBFT_DIR)
INCLUDE = minishell.h

SRC_FILES = main.c \
			builts_utils.c \
			ft_env.c \
			ft_cd.c \
			ft_cd2.c \
			ft_cd3.c \
			ft_cd4.c \
			ft_unset.c \
			ft_echo.c \
			ft_exit.c \
			ft_pwd.c \
			ft_export.c \
			ft_export2.c \
			init.c \
			close_minishell.c \
			environ.c \
			environ2.c \
			environ_utils.c \
			signal.c \
			signal_handlers.c \
			warnings.c \
			sys_home_path.c \
			sys_host_user.c \
			sys_ps1.c \
			sys_utils.c \
			executor.c \
			executor_utils.c \
			exec_utils.c \
			exec_utils2.c \
			error_exit.c \
			redirs_utils.c \
			redirs_utils2.c \
			free_and_close.c \
			wait_pids.c \
			init_parsing.c \
			lexer_parser.c \
			list_utils.c \
			heredoc.c \
			heredoc_utils.c \
			parsing_utils.c \
			parsing_utils2.c \
			expand.c \
			expand_utils.c \
			parsing.c \
			parsing2.c \
			parsing_errors.c \
			jump_table.c \
			jump_table2.c \
			lexer_utils.c \
			data_formats.c

OBJ_DIR = obj/
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: $(LIBFT_A) $(OBJ_DIR) $(NAME)

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
	@echo "\033[0;32mMinishell compiled successfully!!\033[0m"
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT_A) -o $(NAME) $(LIBS)


$(OBJ_DIR)%.o: %.c $(INCLUDE)
	@$(MKDIR) $(dir $@)
	@$(CC) $(FLAGS) $(INCLUDE_FLAG) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) --no-print-directory clean -C $(LIBFT_DIR)

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) --no-print-directory fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re message loading_bar