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
typedef struct s_mshell
{
    char *input;
}   t_mshell;

/*
 * FUNCIONES
 */
// environ
void    ft_environ_init(char **env);
// signal
void    ft_signal_init(void);

// libft
void    ft_putstr_fd(char *str, int fd);
#endif