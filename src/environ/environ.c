#include "minishell.h"

void    ft_environ_init(char **env)
{
    int i;

    i = 0;
    while(env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
    printf("\n");
}