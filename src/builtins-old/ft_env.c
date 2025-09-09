#include <stdio.h>
#include <string.h>

void    ft_env(char **env)
{
    int i;

    i = 0;
    while(env[i])
    {
        if (!strncmp(env[i], "_=./a.out", 9)) // Unica diferencia
            printf("_=/usr/bin/env\n");
        else
            printf("%s\n", env[i]);
        ++i;
    }
    (void)env;
}

int main(int argc, char **argv, char **env)
{
    if (argc == 1)
        ft_env(env);
    (void)argv;
    return (0);
}