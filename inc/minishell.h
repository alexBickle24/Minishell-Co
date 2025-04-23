#ifndef MINISHELL_H
# define MINISHELL_H

/*
 * LIBRERIAS
*/
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>

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
    char *name;
}   t_mshell;

/*
 * FUNCIONES
 */
// signal
void    signal_init(void);

// libft
void    ft_putstr_fd(char *str, int fd);
#endif