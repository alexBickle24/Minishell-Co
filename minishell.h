/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:40:39 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:31:38 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define MLEN_INT 12
# define PROMPT "\001\033[0;32m\002minishell\001\033[0m\002$"
# define PROMPT1 "\033[1;37;43mminishell\033[0m"
# define INTERPRETER_NAME "/bin/bash"
# define NEWLINE_ERR "minishell: syntax error near unexpected token `newline'\n"
# define MIQUOTE_ERR "minishell: syntax error: unclosen quotes\n"
# define MIPIPE_ERR "minishell: syntax error: orfant pipe\n"

# define C_RESET "\001\033[0m\002"
# define C_RED "\001\033[1;31m\002"
# define C_GREEN "\001\033[1;32m\002"
# define C_YELLOW "\001\033[1;33m\002"
# define C_BLUE "\001\033[1;34m\002"
# define C_MAGENTA "\001\033[1;35m\002"
# define C_CYAN "\001\033[1;36m\002"
# define C_WHITE "\001\033[1;37m\002"

# define BOOSTER "\xF0\x9F\x9A\x80 "

extern int						g_signal;

typedef enum e_states
{
	S_INIT,
	S_SIGINT = 2,
	S_SIGQUIT,
	S_EXECUTION,
	S_HEREDOC,
	S_EXECUTION_S
}								t_states;

typedef enum e_type_tocken
{
	T_CMD,
	T_HEREDOC,
	T_HEREDOC_S,
	T_HERE_STR,
	T_INFILE,
	T_APPEND,
	T_OUTFILE,
	T_PIPE,
}								t_filetype;

typedef enum e_lex_states
{
	NO_QUOTES,
	S_QUOTES,
	D_QUOTES,
}								t_quote;

typedef enum e_lastinfo
{
	INIT,
	WORD,
	REDIR,
	OPERATOR,
}								t_info;

typedef struct s_parsing_utils	t_parsing;
typedef struct s_msl			t_msl;
typedef struct s_lex_tockens	t_lex;

typedef struct s_tocken_files
{
	int							type;
	char						*file_name;
	char						ambiguos;
	struct s_tocken_files		*next;
}								t_files;

typedef struct s_tocken_cmds
{
	char						*cmd;
	struct s_tocken_cmds		*next;
}								t_pcmds;

typedef struct s_tocken_subshells
{
	int							index;
	t_files						*files;
	t_files						*redir_in;
	t_files						*redir_out;
	t_pcmds						*pcmds;
	char						**cmd_tb;
	int							error_file;
	int							error_cmd;
	char						**env_tb;
	int							pipe_fds[2];
	pid_t						pid;
	struct s_tocken_subshells	*next;
}								t_tocken;

typedef struct s_parsing_utils
{
	int							parstat;
	int							lexstat;
	int							infstat;
	int							(*lex[6])(t_msl *msl, int *i,
			unsigned char *line, t_parsing *pars);
	unsigned char				sep_op[255];
	unsigned char				*ptr;
	char						dollar_lim[255];
}								t_parsing;

typedef struct s_lex_tockens
{
	int							type;
	char						*raw;
	char						*str;
	size_t						len;
	struct s_lex_tockens		*next;
}								t_lex;

typedef struct s_system_info
{
	char						*user;
	char						*host;
	char						*home;
	char						*pwd;
	char						*g_path;
	char						*ps1_hostuser;
	char						*ps1_path;
	char						*ps1;
}								t_system;

typedef struct s_env
{
	char						*id;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_builts
{
	bool						unst_oldpwd;
}								t_builts;

typedef struct s_msl
{
	t_system					*sys;
	t_env						*own_env;
	t_builts					*builts;
	int							mode;
	int							exit_status;
	char						*clean_line;
	pid_t						msl_pid;
	pid_t						last_process;
	int							total_tockens;
	t_tocken					*tocken;
	t_parsing					*parsing_utils;
	t_lex						*lexer;
	char						pars_err;
}								t_msl;

void							evaluate_line(t_msl *msl,
									unsigned char *clean_line);
void							interactive_mode(t_msl **msl, char **env);
void							minishell_init(t_msl **msl, char **env,
									char mode);
void							init_system(t_msl *msl);
void							set_env_default_values(t_msl *msl);
void							interactive_mode(t_msl **msl, char **env);
void							evaluate_line(t_msl *msl,
									unsigned char *clean_line);
void							free_line(char *line, t_msl *msl);
void							free_own_env(t_msl *msl);
void							free_sys(t_system **sys);
void							free_msl(t_msl **msl);
void							execute_cmd(char *command, char *option,
									int *pipes, char **env);
char							**create_args(char *command, char *option);
int								get_infcmds(t_msl *msl, char **target,
									char *command, char *args);
char							*which_cmd(t_msl *msl, char *command,
									char *args);
void							user_fallbacks(t_msl *msl);
void							user_fallbacks2(t_msl *msl, int fallback);
void							hostname_fallbacks(t_msl *msl, char **target);
int								get_hostnamedir(char **target);
void							get_home(t_msl *msl, char **target);
char							*find_home(char *line);
void							init_pwd(t_msl *msl);
void							hardcoding_home(t_msl *msl, char **target);
void							get_global_path(char **target);
char							put_global_path(char **target, int fd);
void							set_ps1(t_msl *msl, t_system *sys);
char							*set_ps1_hostuser(t_system *sys);
char							*set_ps1_path(t_msl *msl);
char							*get_display_path(t_msl *msl, char *cwd);
void							ft_env_table_to_list(t_msl *msl, char **env_tb);
t_env							*search_creat_envnode(t_msl *msl,
									char **env_tb);
void							set_shlvl(t_msl *msl);
void							check_home_message(t_msl *msl);
void							set_pwd(t_msl *msl);
void							set_path(t_msl *msl);
void							set_lessopen(t_msl *msl);
void							set_lessclose(t_msl *msl);
void							set_last_arg_init(t_msl *msl);
char							*ft_get_env_id(char *env_line);
char							*ft_get_env_value(char **env, char *id);
t_env							*search_id_node(t_msl *msl, char *id);
t_env							*list_new_ownenv(char *id, char *value);
void							list_addback_env(t_env *node, t_env **env);
void							signal_init(void);
void							signal_init_childs(void);
void							signal_init_heredoc(void);
void							sig_handler(int signals);
void							sig_child_handler(int signal);
void							sig_heredoc_handler(int signal);
char							**ft_pcmds_to_table(t_pcmds *pcmds);
char							**ft_env_to_table(t_env *env);
char							**create_table(t_pcmds *list);
char							**create_table1(t_env *list);
void							ft_shlvl_warning(int str);
void							ft_error_home(void);
void							ft_errorargs(char *arg);
void							ft_errormini(void);
t_parsing						*init_parsing(t_msl *msl);
void							init_sep_op(unsigned char *sep_op);
void							init_dollar_lim(char *dollar_limits,
									unsigned char *sep_op);
void							init_jump_table(int (**f)(t_msl *, int *,
										unsigned char *, t_parsing *));
t_tocken						*list_new_tocken(int position);
t_files							*list_new_files(char *file, int type,
									char ambiguos);
t_pcmds							*list_new_pcmds(char *cmd);
void							list_addback_tocken(t_tocken **list,
									t_tocken *new_node);
void							list_addback_infiles(t_files **list,
									t_files *new_node);
void							list_addback_pcmds(t_pcmds **list,
									t_pcmds *new_node);
void							set_parsdefaultvals(t_msl *msl);
void							free_lexer(t_msl *msl, char all);
void							lexer_parser(t_msl *msl, unsigned char *line);
void							addback_lex(t_msl *msl, t_lex *node);
t_lex							*lex_newnode(int type, char *raw);
void							ft_error_unexpect(char *simbol);
int								get_unexpexted_errors1(int type, int infstat);
int								redir_out(t_msl *msl, int *i,
									unsigned char *line, t_parsing *pars);
int								redir_in(t_msl *msl, int *i,
									unsigned char *line, t_parsing *pars);
void							redir_in_extracases(int *i, unsigned char *line,
									t_parsing *pars);
int								pipe_op(t_msl *msl, int *i, unsigned char *line,
									t_parsing *pars);
int								spaces(t_msl *msl, int *i, unsigned char *line,
									t_parsing *pars);
int								info(t_msl *msl, int *i, unsigned char *line,
									t_parsing *pars);
int								d_quotes(t_msl *msl, int *i,
									unsigned char *line, t_parsing *pars);
int								s_quotes(t_msl *msl, int *i,
									unsigned char *line, t_parsing *pars);
int								quotes(t_msl *msl, int *i, unsigned char *line,
									t_parsing *pars);
void							create_newlex(t_msl *msl, int *i,
									unsigned char *line, t_parsing *pars);
void							manage_last_state(t_msl *msl,
									t_parsing *parser);
int								have_quotes(char *str);
char							*clean_quotes(char *str);
char							*jump_caracter(char *str, char caracter);
int								is_space(char c);
void							jump_separator(char **str);
char							check_clean_quotes(t_msl *msl, char *str,
									char clean);
char							check_clean_squotes(t_msl *msl, char *str,
									char clean);
char							check_clean_dquotes(t_msl *msl, char *str,
									char clean);
void							clean_expand_add_toexecuter(t_msl *msl);
void							adding_files(t_msl *msl, t_tocken *current,
									t_lex *lex);
void							adding_cmds(t_msl *msl, t_tocken *current,
									t_lex *lex);
void							adding_files(t_msl *msl, t_tocken *current,
									t_lex *lex);
void							adding_here(t_msl *msl, t_tocken *current,
									t_lex *lex);
void							adding_tocken(t_msl *msl, t_tocken **current,
									t_lex *lexer);
void							loop_cmd_split(char **str, char **start,
									t_msl *msl, t_tocken *cur);
char							check_ambiguos_file(char *str, t_msl *msl);
char							new_line_err(t_msl *msl);
void							create_heredoc_nwlerr(t_msl *msl, t_lex *lex);
char							check_nwl_error(t_msl *msl);
void							vars_interpolation(char **str, t_msl *msl,
									size_t *len);
void							dollar_expansion(char **str, int *i,
									size_t *len, t_msl *msl);
void							dollar_expansion2(char **str, int *i,
									size_t *len, t_msl *msl);
void							replace_dollar(char **str, int *i, size_t *len,
									t_msl *msl);
void							concatenate_strings(char **str, int *i,
									size_t *len, char *replace);
void							concatenate_strings2(char **str, int *i,
									size_t *len, char *replace);
void							expand_vars(char **str, int *i, size_t *len,
									t_msl *msl);
void							home_case(char **str, t_msl *msl);
char							*create_heredoc(t_msl *msl, char *delimiter,
									char mode);
char							*new_file_name(char *path);
void							set_heredocs_modes(char *modes, char *delimiter,
									char sangria);
void							heredoc_child_process(t_msl *msl, int fd,
									char *delimiter, char *modes);
void							heredoc_loop(t_msl *msl, char *delimiter,
									int fd, char *modes);
void							write_line_in_heredoc(char *line, int fd,
									t_msl *msl, char *modes);
int								write_dollar_cases(char *t_line, t_msl *msl,
									int fd, int i);
unsigned int					write_env(char *line, int fd,
									unsigned int count, t_msl *msl);
void							ft_hwarningexit(char *delimiter);
char							*create_here_str(char *line);
void							executer(t_msl *msl);
void							only_builtin(t_msl *msl);
void							execute_orders(t_msl *msl);
void							execute_childs(t_tocken *c_tocken, t_msl *msl);
void							child_process(int s_builtin, t_tocken *c_tocken,
									t_msl *msl);
void							cmd_vs_builtin(t_msl *msl, t_tocken *c_tocken,
									int building);
void							evaluate_tocken_cmds_errors(t_tocken *c_tocken,
									t_msl *msl);
int								handle_direct_path(t_tocken *c_tocken);
char							*check_path(char *x_file, char **env);
char							*find_exe_file(char **posible_paths,
									char *x_file);
char							*get_env_value(const char *key_value,
									char **env);
int								is_builtin(t_tocken *tocken);
void							exec_cmd(t_tocken *c_tocken);
int								father_builtin(t_msl *msl, t_tocken *c_tocken,
									int builtin);
void							create_pipes(t_tocken *c_tocken);
void							check_create_redirs(t_tocken *c_tocken,
									t_files *files_list);
int								check_out_redirs(t_tocken *c_tocken,
									t_files *cur_f);
int								tunel_in_file(char *file);
int								tunel_out_file(char *file, char append);
int								pipe_forward(int *pipe_reference, int pipe_port,
									int fd);
void							fordward_in(t_tocken *c_tocken);
void							fordward_out(t_tocken *c_tocken);
void							ft_exterror_exes(char *file, char is_directory);
void							ft_exterror_cmd(char *file);
void							ft_exterrno(void);
void							ft_error_redirs(char *file, char ambiguos);
void							ft_errerrno(void);
void							wait_childs1(t_msl *msl);
void							wait_childs3(t_msl *msl);
void							wait_childs4(t_msl *msl, pid_t pid_heredoc);
int								wait_childs5(pid_t pid_child);
int								ft_getpid(void);
void							free_tocken_files(t_tocken *tocken);
void							free_tocken_cmds(t_tocken *tocken);
void							free_tockens(t_msl *msl);
void							close_fds(int *pipe_ports);
void							ft_free_table(char **ptr);
char							*set_last_arg(t_msl *msl);
void							create_last_arg(t_msl *msl, char *target);
void							ft_freeptr(void *ptr);
int								ft_argscounter(t_pcmds *pcmds);
int								ft_check_id(t_msl *msl, char *id, char *value);
int								ft_check_isalnum_id(char *cmd, char *id);
int								ft_check_export(char *cmd);
t_env							*ft_lstnew_env(char *id, char *value,
									int alloc);
int								ft_cd(t_msl *msl, t_pcmds *pcmds);
int								one_arg_cases(t_msl *msl, t_pcmds *pcmds);
void							ft_cderrors(int value, char *path, char flag);
int								search_first_parent(t_msl *msl);
int								go_home(t_msl *msl);
int								stay_case(char *path, t_msl *msl);
int								parent_case(char *path, t_msl *msl);
int								go_oldpwd(t_msl *msl);
void							set_olpwd(t_msl *msl, char *oldpath);
void							set_pwd_oldpwd(t_msl *msl, char *oldpath);
int								check_parent(t_msl *msl);
void							unlink_message(void);
int								is_broken_pwd(t_msl *msl);
int								edit_pwd(t_msl *msl, char *path, char message);
int								go_parent(t_msl *msl, char *path);
int								go_root(t_msl *msl, char *pwd);
int								go_firstparent(t_msl *msl, char *pwd);
int								ft_echo(t_tocken *tocken);
void							print_args(t_pcmds *args);
void							ft_exit(t_msl *msl, t_tocken *tocken,
									char father);
void							exit_onearg(t_msl **msl, t_pcmds *pcmds);
int								ft_unset(t_msl *msl, t_tocken *tocken);
void							remove_envnode(t_env *node, t_env **begin,
									t_env *prev_node);
void							free_envnode(t_env *node);
int								ft_pwd(t_msl *msl);
int								ft_env(t_msl *msl, t_pcmds *pcmds);
int								ft_export(t_msl *msl, t_pcmds *pcmds);
int								ft_add_env(t_msl *msl, char *cmd);
char							*ft_get_one_env_value(char *env, char *id);
void							ft_print_env(t_env *own_env);
t_env							*ft_sort_env(t_env *own_env);

#endif
