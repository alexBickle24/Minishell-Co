#include "minishell.h"

int main(int argc, char **argv, char **env)
{
    t_mshell msl;

    if (argc != 1)
        return (1);
    signal_init();
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
    (void)env;
    return(0);
}