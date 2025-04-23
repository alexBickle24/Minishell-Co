#include "minishell.h"

int main(int argc, char **argv, char **env)
{
    t_mshell msl;

    if (argc != 1)
        return (1);
    signal_init();
    printf("End signal!\n");
    (void)msl;
    (void)argv;
    (void)env;
    return(0);
}