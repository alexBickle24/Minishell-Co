

#ifndef MINISHELL_H
# define MINISHELL_H

/*
 * LIBRERIAS
*/

# include "libft/libft.h"
//readline
# include <readline/readline.h>
# include <readline/history.h>
//tipicas
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>
//señales / bloqueos / errores
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
//ficheros
# include <fcntl.h> 


/*
 * DEFINES
*/
# define MLEN_INT 12
# define PROMPT "\001\033[0;32m\002minishell\001\033[0m\002$"
# define INTERPRETER_NAME "minishell"

//Para el manejo de señales
extern int  g_signal;


typedef enum e_states
{
    S_INIT,    // Señal de inicio
    S_SIGINT,    // Señal Ctrl + C
    S_SIGQUIT,   // Señal inicio
    // *Estados de la minishell (opcionales)
    S_LEXING,    // *Indica que estamos en el proceso de lexing
    S_PARSING,   // *Indica que estamos en el proceso de parsing
    S_IN_HEREDOC,// *Indica que estamos en un heredoc
    S_END_HEREDOC,// *Indica que hemos terminado un heredoc
    S_EXECUTING, // *Indica que estamos en el proceso de ejecución
}   t_states;


typedef enum e_type_tocken
{
    CMD,
    HEREDOC,
    INFILE,
    APPEND,
    OUTFILE,
    PIPE,
}   t_filetype;

/*
 * ESTRUCTURAS
 */

typedef struct s_tocken_files
{
    int type; //HEREDOC, INFILE, APPEND, OUTFILE
    char *file_name; //NOMBRE DEL ARCHIVO
    int fd; //*FD DEL ARCHIVO (opcional)
    struct s_tocken_files *next;
}   t_files;

typedef struct s_tocken_cmds
{
    char *cmd;
    struct s_tocken_cmds *next;
}   t_pcmds;

typedef struct s_tocken_subshells
{
    int index;//numero de tocken, quizas no lo necesitemos
    t_files *files;//lista de in files se recorre y llena redir in y redir out o error 
    t_files *redir_in;
    t_files *redir_out;
    t_pcmds *pcmds; //lista de comandos, si es NULL no hay comando  
    char **cmd_tb; //tabla de comandos para execve
    int error_file;//flag para manejar el exit desde el proceso hijo
    int error_cmd; //Puede ser 0, 1 o 2 manej los mensajes de error
    char **env_tb;//esto no es lo mas eficiente porque la guardao en cada tocken pero es comodo
    int pipe_fds[2];//con las bonus tendria que ser tocken operands
    pid_t pid;
    struct s_tocken_subshells *next;
}   t_tocken;


typedef struct s_env
{
    char *id;
    char *value;
    struct s_env *next;
}   t_env;
 
typedef struct s_parsing_utils
{
    //el tipo de tocken que stamos creando
    int type; //Pr defecto es CMD
    //Para saber si estamos dentro de comillas o no
    bool in_quotes;
    //Caracteres de separacion
    char separators[255];

    //Cuando no estamos en quotes son separadores y crean lex tockens
    char special_op[255];//>0 si es limitante =0
    //Funciones de la JUMP_TABLE.
    
    char *start;//Puntero al inicion de la palabra
    char *end;//Puntero al final de la palabra

    //Para la expansion de variables
    char dollar_lim[255];//>0 limitante =0 no lo es

}   t_parsing;

typedef struct s_lex_tockens
{
    int type;
    char *str;
    struct s_lexer *next;

}   t_lex;

typedef struct s_msl
{
    t_env   *own_env;
    t_states states;//*
    int exit_status; 
    char *clean_line;
    pid_t msl_pid;
    pid_t last_process;
    int total_tockens;
    t_tocken *tocken;
    t_parsing *parsing_utils;
    t_lex *lexer;
}   t_msl;





/*
    * /////////////////////////////////FUNCIONES///////////////////////////
*/

//testing exec
void parser1(t_msl *msl);

//debugin
void print_tockens(t_msl *msl);
void print_files(t_files *files);
void print_pcmds(t_pcmds *pcmds);
void print_tockens_info(t_tocken *tockens);
void print_own_env(t_env *env);
void print_parsing_info(t_parsing *parsing);
void print_msl(t_msl *msl);//

//minishell init
void    minishell_init(t_msl **msl, char **env, char **argv);
void    interpreter_mode(t_msl *msl);

//minishell close
void    free_own_env(t_msl *msl);
void    free_msl(t_msl **msl);

// environ
void    env_init(t_msl *msl, char **inherid_env, char **argv);
void    ft_env_table_to_list(t_msl *msl, char **env_tb);
void    set_env_default_values(t_msl *msl, char **argv);
void    set_shlvl(t_msl *msl);

//environ utils
char    *ft_get_env_id(char *env_line);
char    *ft_get_env_value(char **env, char *id);
t_env *search_id_node(t_msl *msl, char *id);
t_env *list_new_ownenv(char *id, char *value);
void  list_add_back_env(t_env *node, t_env **env);
char *get_minipath(char **argv);


// signals
void    signal_init(void);
void    signal_init_childs(void);
void    signal_init_heredoc(void);
void    sig_handler(int signals);
void    sig_child_handler(int signal);
void    sig_heredoc_handler(int signal);

//general utils;
char    **ft_pcmds_to_table(t_pcmds *pcmds);
char    **ft_env_to_table(t_env *env);

//warnings and errors
void ft_shlvl_warning(char *str);



/////////////////////////LEXER AND PARSING////////////////////////


//init parsing
t_parsing *init_parsing(t_msl *msl);
void init_separators(char *separators);
void init_special_op(char *operators);
void init_dollar_lim(char *dollar_limits, char *separators, char *operators);

//Create_tockens
t_tocken    *list_new_tocken(int position);
t_files     *list_new_files(char *file, int type);
t_pcmds     *list_new_pcmds(char *cmd);
void        list_addback_tocken(t_tocken **list, t_tocken *new_node);
void        list_addback_infiles(t_files **list, t_files *new_node);
void        list_addback_pcmds(t_pcmds **list, t_pcmds *new_node);

//parsing_utils
int     have_quotes(char *str);
char    *clean_quotes(char *str);
char    *jump_caracter(char *str, char caracter);
int     is_space(char c);


// heredoc
char    *create_heredoc(t_msl *msl, char *delimiter, char mode);
char	*new_file_name(char *path);
void    heredoc_child_process(t_msl *msl, int fd, char *delimiter, char *modes);
void    heredoc_loop(t_msl *msl, char *delimiter, int fd, char *modes);
void    write_line_in_heredoc(char *line, int fd, t_msl *msl, char *modes);
int     write_dollar_cases(char *t_line, t_msl *msl, int fd, int i);
unsigned int	write_env(char *line, int fd, unsigned int count, char **env);
void ft_hwarningexit(char *delimiter);



///////////////////////////EXECUTION//////////////////////////////////
//exec
void executer(t_msl *msl);
void only_builtin(t_msl *msl);
void execute_orders(t_msl *msl);
void execute_childs(t_tocken *c_tocken, t_msl *msl);
void cmd_vs_builtin(t_tocken *c_tocken, int building);

//exec_utils
void evaluate_tocken_cmds_errors(t_tocken *c_tocken, t_msl *msl);
char *check_path(char *x_file, char **env);
char *find_exe_file(char **posible_paths, char *x_file);
char *get_env_value(const char *key_value, char **env);
int is_builtin(t_tocken *tocken);
void exec_cmd(t_tocken *c_tocken);
void exec_builtin(t_msl *msl, t_tocken *c_tocken, int building);
void create_pipes(t_tocken *c_tocken);

//redirs utils
void check_create_redirs(t_tocken *c_tocken, t_files *files_list);
int tunel_in_file(char *file);
int tunel_out_file(char *file, char append);//hay que cambiarla para el modo appends
int pipe_forward(int *pipe_reference, int pipe_port, int fd);
void fordward_in(t_tocken *c_tocken);
void fordward_out(t_tocken *c_tocken);

//error_messages and exit
void ft_exterror_exes(char *file);
void ft_exterror_cmd(char *file);
void ft_exterrno(void);
void ft_error_redirs(char *file);
void ft_errerrno(void);

//wait, pids
void    wait_childs1(t_msl *msl);
void    wait_childs2(t_msl *msl);
void    wait_childs3(t_msl *msl);
void    wait_heredoc(void);
int     ft_getpid(void);

//free_and_close
void	free_tocken_files(t_tocken *tocken);
void    free_tocken_cmds(t_tocken *tocken);
void    free_tockens(t_msl *msl);
void    close_fds(int *pipe_ports);
void    ft_free_table(char **ptr);

#endif