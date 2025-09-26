

#ifndef MINISHELL_H
#define MINISHELL_H

/*
 * LIBRERIAS
 */

#include "libft/libft.h"
// readline
#include <readline/readline.h>
#include <readline/history.h>
// tipicas
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
// señales / bloqueos / errores
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
// ficheros
#include <fcntl.h>
#include <sys/stat.h>

/*
 * DEFINES
 */

#define MLEN_INT 12
#define PROMPT "\001\033[0;32m\002minishell\001\033[0m\002$"
#define PROMPT1 "\033[1;37;43mminishell\033[0m"
#define INTERPRETER_NAME "/bin/bash"
#define NEWLINE_ERR "minishell: syntax error near unexpected token `newline'\n"
#define MIQUOTE_ERR "minishell: syntax error: unclosen quotes\n"
#define MIPIPE_ERR "minishell: syntax error: orfant pipe\n"


//define de colors
#define C_RESET  "\001\033[0m\002"
#define C_RED    "\001\033[1;31m\002"
#define C_GREEN  "\001\033[1;32m\002"
#define C_YELLOW "\001\033[1;33m\002"
#define C_BLUE   "\001\033[1;34m\002"
#define C_MAGENTA "\001\033[1;35m\002"
#define C_CYAN   "\001\033[1;36m\002"
#define C_WHITE  "\001\033[1;37m\002"

//define booster
#define BOOSTER "\xF0\x9F\x9A\x80 "
// #define BOOSTER "€ "

// Para el manejo de señales
extern int g_signal;


/*
	ENUMS:
*/

// estado de señales
typedef enum e_states
{
	S_INIT,	   // Señal de inicio
	S_SIGINT = 2,  // Señal Ctrl + C
	S_SIGQUIT, // Señal inicio
	S_EXECUTION,
	S_HEREDOC,
	S_EXECUTION_S
} t_states;

// esrdos de parseo de tokens
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
} t_filetype;

// estado de lexerizacion
typedef enum e_lex_states
{
	NO_QUOTES,
	S_QUOTES,
	D_QUOTES,
} t_quote;

//para la deteccion de errores de operadores (ESTADO DE LA INFORMACION)
typedef enum e_lastinfo
{
	INIT,
	WORD,
	REDIR,
	OPERATOR,
} t_info;

/*
 * ESTRUCTURAS
 */

typedef struct s_parsing_utils t_parsing;
typedef struct s_msl t_msl;
typedef struct s_lex_tockens t_lex;

/////////////////////////////////////////////////////////// ESTRCUTURAS DE EJECUCION //////////////////////////////////////


typedef struct s_tocken_files
{
	int type;
	char *file_name;
	char ambiguos;
	struct s_tocken_files *next;
} t_files;

typedef struct s_tocken_cmds
{
	char *cmd;
	struct s_tocken_cmds *next;
} t_pcmds;

typedef struct s_tocken_subshells
{
	int index;		// numero de tocken, quizas no lo necesitemos
	t_files *files; // lista de in files se recorre y llena redir in y redir out o error
	t_files *redir_in;
	t_files *redir_out;
	t_pcmds *pcmds; // lista de comandos, si es NULL no hay comando
	char **cmd_tb;	// tabla de comandos para execve
	int error_file; // flag para manejar el exit desde el proceso hijo
	int error_cmd;	// Puede ser 0, 1, 2  o 3 manej los mensajes de error
	char **env_tb;	 // esto no es lo mas eficiente porque la guardao en cada tocken pero es comodo
	int pipe_fds[2]; // con las bonus tendria que ser tocken operands
	pid_t pid;
	struct s_tocken_subshells *next;
} t_tocken;

////////////////////////////////////////////////////// ESTRCUTURAS DE PARSEO ///////////////////////////////////////////////////


typedef struct s_parsing_utils
{
	int	parstat;
	int	lexstat;
	int	infstat;
	int	(*lex[6])(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
	unsigned char	sep_op[255];
	unsigned char	*ptr;
	char	dollar_lim[255];
}	t_parsing;

typedef struct s_lex_tockens
{
	int						type;
	char					*raw;
	char					*str;
	size_t					len;
	struct s_lex_tockens	*next;
} t_lex;

///////////////////////////////////////////////////////////////// ESTRCUTURAS DE SISTEMA Y CABEZERA ///////////////////////////////////////////

//Esta estrcutura tiene informacion relevante de la maquina
//sirve para un funcionamiento correcto del interprete cuando
//nos conectamos ssh a la tty y o para ejecutar la minishell en local
typedef struct s_system_info
{
	char	*user;
	char	*host;
	char	*home;
	char	*pwd;
	char	*g_path;
	char	*ps1_hostuser;
	char	*ps1_path;
	char	*ps1;
}	t_system;

typedef struct s_env
{
	char			*id;
	char			*value;
	struct s_env	*next;
} t_env;

typedef struct s_builts
{
	bool	unst_oldpwd;
}	t_builts;

typedef struct s_msl
{
	t_system	*sys;
	t_env		*own_env;
	t_builts	*builts;
	int			mode;
	int			exit_status;
	char		*clean_line;
	pid_t		msl_pid;
	pid_t		last_process;
	int			total_tockens;
	t_tocken	*tocken;
	t_parsing	*parsing_utils;
	t_lex		*lexer;
	char		pars_err;
} t_msl;

/*
 * PROTOTIPOS
*/

///////////////// INIT Y CREACION DE ENTRONO DE SISTEMA///////////////////

// debugin
void	print_tockens(t_msl *msl);
void	print_files(t_files *files);
void	print_pcmds(t_pcmds *pcmds);
void	print_tockens_info(t_tocken *tockens);
void	print_own_env(t_env *env);
void	print_parsing_info(t_parsing *parsing);
void	print_lex(t_lex *lexer, t_parsing *parser);
void	print_msl(t_msl *msl); //
void	print_parser_state(t_parsing *parser, unsigned char c, int i);

// minishell init
void	minishell_init(t_msl **msl, char **env);
void	init_system(t_msl *msl);
void	set_env_default_values(t_msl *msl);
void	init_builts(t_msl *msl);
void	interactive_mode(t_msl *msl);
void	evaluate_line(t_msl *msl, unsigned char *clean_line);

// minishell close
void	free_own_env(t_msl *msl);
void	free_sys(t_system **sys);
void	free_msl(t_msl **msl);

//sytem_info
void	execute_cmd(char *command, char *option, int *pipes, char **env);
char	**create_args(char *command, char *option);
int		get_infcmds(t_msl *msl, char **target, char *command, char *args);
char	*which_cmd(t_msl *msl, char *command, char *args);
void	user_fallbacks(t_msl *msl);
void	user_fallbacks2(t_msl *msl, int fallback);
void	hostname_fallbacks(t_msl *msl, char **target);
int		get_hostnamedir(char **target);
void	get_home(t_msl *msl, char **target);
char	*find_home(char *line);
void	init_pwd(t_msl *msl);
void	hardcoding_home(t_msl	*msl, char	**target);
void	get_global_path(char **target);
char	put_global_path(char **target, int fd);
void	set_ps1(t_msl *msl, t_system *sys);
char	*set_ps1_hostuser(t_system *sys);
char	*set_ps1_path(t_msl *msl);
char	*get_display_path(t_msl *msl, char *cwd);

// environ
void	ft_env_table_to_list(t_msl *msl, char **env_tb);
t_env	*search_creat_envnode(t_msl *msl, char **env_tb);
void	set_shlvl(t_msl *msl);
void	check_home_message(t_msl *msl);
void	set_pwd(t_msl *msl);
void	set_path(t_msl *msl);
void	set_lessopen(t_msl *msl);
void	set_lessclose(t_msl *msl);
void	set_last_arg_init(t_msl *msl);

// environ utils
char	*ft_get_env_id(char *env_line);
char	*ft_get_env_value(char **env, char *id);
t_env	*search_id_node(t_msl *msl, char *id);
t_env	*list_new_ownenv(char *id, char *value);
void	list_addback_env(t_env *node, t_env **env);

// signals
void	signal_init(void);
void	signal_init_childs(void);
void	signal_init_heredoc(void);
void	sig_handler(int signals);
void	sig_child_handler(int signal);
void	sig_heredoc_handler(int signal);

// general utils;
char	**ft_pcmds_to_table(t_pcmds *pcmds);
char	**ft_env_to_table(t_env *env);
char	**create_table(t_pcmds *list);
char	**create_table1(t_env *list);

// warnings and errors
void	ft_shlvl_warning(int	str);
void	ft_error_home(void);
void	ft_errorargs(char *arg);
void	ft_errormini(void);

/////////////////////////LEXER AND PARSING////////////////////////

// init parsing
t_parsing	*init_parsing(t_msl *msl);
void	init_sep_op(unsigned char *sep_op);
void	init_dollar_lim(char *dollar_limits, unsigned char *sep_op);
void	init_jump_table(int (**f)(t_msl *, int *, unsigned char *, t_parsing *));

// Create_tockens
t_tocken	*list_new_tocken(int position);
t_files		*list_new_files(char *file, int type, char ambiguos);
t_pcmds		*list_new_pcmds(char *cmd);
void	list_addback_tocken(t_tocken **list, t_tocken *new_node);
void	list_addback_infiles(t_files **list, t_files *new_node);
void	list_addback_pcmds(t_pcmds **list, t_pcmds *new_node);

//Lexer (momentaneo)
void	set_parsdefaultvals(t_msl *msl);
void	free_lexer(t_msl *msl, char all);
void	lexer_parser(t_msl *msl, unsigned char *line);
void	addback_lex(t_msl *msl, t_lex *node);
t_lex	*lex_newnode(int type, char *raw);
void	ft_error_unexpect(char *simbol);
int		get_unexpexted_errors1(int type, int infstat);
int		redir_out(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
int		redir_in(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
void	redir_in_extracases(int *i, unsigned char *line, t_parsing *pars);
int		pipe_op(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
int		spaces(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
int		info(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
int		d_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
int		s_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
int		quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
void	create_newlex( t_msl *msl, int *i, unsigned char *line, t_parsing *pars);
void	manage_last_state(t_msl *msl, t_parsing *parser);

// parsing_utils
int		have_quotes(char *str);
char	*clean_quotes(char *str);
char	*jump_caracter(char *str, char caracter);
int		is_space(char c);
void	jump_separator(char **str);
char	check_clean_quotes(t_msl *msl, char *str, char clean);
char	check_clean_squotes(t_msl *msl, char *str, char clean);
char	check_clean_dquotes(t_msl *msl, char *str, char clean);

//adding and expansion
void	clean_expand_add_toexecuter(t_msl *msl);
void	adding_files(t_msl *msl, t_tocken *current, t_lex *lex);
void	adding_cmds(t_msl *msl, t_tocken *current, t_lex *lex);
void	adding_files(t_msl *msl, t_tocken *current, t_lex *lex);
void	adding_here(t_msl *msl, t_tocken *current, t_lex *lex);
void	adding_tocken(t_msl *msl, t_tocken **current, t_lex *lexer);

//adding_extra->->_>_->->->
void	loop_cmd_split(char **str, char **start, t_msl *msl, t_tocken *cur);
char	check_ambiguos_file(char *str, t_msl *msl);

//fix error
char	new_line_err(t_msl *msl);
void	create_heredoc_nwlerr(t_msl *msl, t_lex *lex);
char	check_nwl_error(t_msl *msl);


//dollar expansion
void	vars_interpolation(char **str, t_msl *msl, size_t *len);
void	dollar_expansion(char **str, int *i, size_t *len, t_msl *msl);
void	dollar_expansion2(char **str, int *i, size_t *len, t_msl *msl);
void	replace_dollar(char **str, int *i, size_t *len, t_msl *msl);
void	concatenate_strings(char **str, int *i, size_t *len, char *replace);
void	concatenate_strings2(char **str, int *i, size_t *len, char *replace);
void	expand_vars(char **str, int *i, size_t *len, t_msl *msl);
void	home_case(char **str, t_msl *msl);

// heredoc
char	*create_heredoc(t_msl *msl, char *delimiter, char mode);
char	*new_file_name(char *path);
void	set_heredocs_modes(char *modes, char *delimiter, char sangria);
void	heredoc_child_process(t_msl *msl, int fd, char *delimiter, char *modes);
void	heredoc_loop(t_msl *msl, char *delimiter, int fd, char *modes);
void	write_line_in_heredoc(char *line, int fd, t_msl *msl, char *modes);
int		write_dollar_cases(char *t_line, t_msl *msl, int fd, int i);
unsigned int	write_env(char *line, int fd, unsigned int count, t_msl *msl);
void	ft_hwarningexit(char *delimiter);
char	*create_here_str(char *line);

///////////////////////////EXECUTION//////////////////////////////////

// exec
void	executer(t_msl *msl);
void	only_builtin(t_msl *msl);
void	execute_orders(t_msl *msl);
void	execute_childs(t_tocken *c_tocken, t_msl *msl);
void	cmd_vs_builtin(t_msl *msl, t_tocken *c_tocken, int building);

// exec_utils
void	evaluate_tocken_cmds_errors(t_tocken *c_tocken, t_msl *msl);
int		handle_direct_path(t_tocken *c_tocken);
char	*check_path(char *x_file, char **env);
char	*find_exe_file(char **posible_paths, char *x_file);
char	*get_env_value(const char *key_value, char **env);
int		is_builtin(t_tocken *tocken);
void	exec_cmd(t_tocken *c_tocken);
int		father_builtin(t_msl *msl, t_tocken *c_tocken, int builtin);
void	create_pipes(t_tocken *c_tocken);

// redirs utils
void	check_create_redirs(t_tocken *c_tocken, t_files *files_list);
int		tunel_in_file(char *file);
int		tunel_out_file(char *file, char append); // hay que cambiarla para el modo appends
int		pipe_forward(int *pipe_reference, int pipe_port, int fd);
void	fordward_in(t_tocken *c_tocken);
void	fordward_out(t_tocken *c_tocken);

// error_messages and exit
void	ft_exterror_exes(char *file, char is_directory);
void	ft_exterror_cmd(char *file);
void	ft_exterrno(void);
void	ft_error_redirs(char *file, char ambiguos);
void	ft_errerrno(void);

// wait, pids
void	wait_childs1(t_msl *msl);
void	wait_childs2(t_msl *msl);
void	wait_childs3(t_msl *msl);
void	wait_childs4(t_msl *msl, pid_t pid_heredoc);
int		wait_childs5(pid_t pid_child);
void	wait_heredoc(void);
int		ft_getpid(void);

// free_and_close
void	free_tocken_files(t_tocken *tocken);
void	free_tocken_cmds(t_tocken *tocken);
void	free_tockens(t_msl *msl);
void	close_fds(int *pipe_ports);
void	ft_free_table(char **ptr);

//set last arg
char	*set_last_arg(t_msl *msl);
void	create_last_arg(t_msl *msl, char *target);

///////////////////////////BUILTINS///////////////////////////////////
int	ft_pwd(t_msl *msl);
int	ft_env(t_msl *msl, t_pcmds *pcmds);
void	ft_unset(t_msl *msl);
// Utils
void	ft_freeptr(void *ptr);
char *ft_cdpath(t_msl *msl);
int	ft_tokencounter(t_msl *msl);//hay que editarla porque sino simepre cuneta el primero 
void	ft_edit_env(t_msl *msl, char *id, char *value);
void	ft_delete_env(t_msl *msl, char *id);
int		ft_argscounter(t_pcmds *pcmds);

//cd
int		ft_cd(t_msl *msl, t_pcmds *pcmds);
int		one_arg_cases(t_msl *msl, t_pcmds *pcmds);
void	ft_cderrors(int value, char *path, char flag);
int		search_first_parent(t_msl *msl);
int		go_home(t_msl *msl);
int		stay_case(char *path, t_msl *msl);
int		parent_case(char *path, t_msl *msl);
int		go_oldpwd(t_msl *msl);
void	set_olpwd(t_msl *msl, char *oldpath);
void	set_pwd_oldpwd(t_msl *msl, char *oldpath);
int		check_parent(t_msl *msl);
void	unlink_message(void);
int		is_broken_pwd(t_msl *msl);
int		edit_pwd(t_msl *msl, char *path, char message);
int		go_parent(t_msl *msl, char *path);
int 	go_root(t_msl *msl, char *pwd);
int		go_firstparent(t_msl *msl, char *pwd);


#endif
