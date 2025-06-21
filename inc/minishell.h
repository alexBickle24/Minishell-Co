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
    S_INIT, // Señal inicio
    S_SIGINT // Señal Ctrl + C
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
char    *ft_get_env(char **env, char *id);

// signal
void    ft_signal_init(void);

// libft
void    ft_putstr_fd(char *str, int fd);
int     ft_strlen(char *str);
int     ft_strncmp(char *s1, char *s2, int n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	ft_putchar_fd(char c, int fd);



// heredoc
char            *create_heredoc(char *string, char *h_delimiter, char **env);
void	        write_line_in_heredoc(char *line, int fd_inter, char **env);
char	        *trim_line(char *line);
unsigned int	write_env(char *line, int fd, unsigned int count, char **env);
char	        *get_env_value(const char *key_value, char **env);

#endif