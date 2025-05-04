#ifndef MINISHELL_H
# define MINISHELL_H

/*
 * LIBRERIAS
*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

/*
 * DEFINES
 */
extern int  g_signal;

typedef enum e_signal
{
    S_INIT // Señal inicio
}   t_signal;

/*
 * ESTRUCTURAS
 */
typedef struct s_env
{
    char *id;
    char **value;
} t_env;

typedef struct s_mshell
{
    char *input;
    t_env   *own_env;
}   t_mshell;

/*
 * FUNCIONES
 */
// environ
void    ft_environ_init(char **env);
void    ft_get_env(char **env, char *id);

// signal
void    ft_signal_init(void);

// libft
void    ft_putstr_fd(char *str, int fd);
int     ft_strlen(char *str);
int     ft_strncmp(char *s1, char *s2, int n);
#endif