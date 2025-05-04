#include "minishell.h"
/*
 * env: Es lo mismo que extern char **environ
  *         env[i] = "PATH=/usr/bin" (ejemplo)
  *         env[N] == NULL
 */
int main(int argc, char **argv, char **env)
{
    t_mshell msl;

    if (argc != 1)
        return (1);
    ft_environ_init(env);
    printf("\n\n");
    ft_get_env(env, "PATH");
    ft_signal_init();
    while (1)
    {
        msl.input = readline("> "); //Muestra un prompt
        if (!msl.input)
        {
            printf("exit\n");
            break ;
        }
        if (*msl.input)
            add_history(msl.input); //Guarda (char*) en el historial
        free(msl.input);
    }
    (void)argv;
    return(0);
}