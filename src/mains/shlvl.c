
#include "../inc/minishell.h"

int g_signal = S_INIT;

/*
    Main para testeo de SHLVL.
    Execve es el ejecutador de programas de Unix.
    Los argumentos que le pasamos a un main hacen que cuando ejecutemos
    un programa, este reciba esos argumentos en sus variables argv y env.
    Execve es el encargado de ejecutar el programa desde la shell 
    y le pasa estos argumentos.
    Por lo que los argumentos que pasemos desde nuestra shell tendran que ser 
    los que haya despues de hacer export y unset. La configuracion de los valores por defecto
    se hace cuando se inicia la shell.
*/

int main(int argz, char **argv, char **env)
{
    (void)argz;
    (void)argv;

    char *shlvl;

    shlvl = get_env_value("SHLVL", env);
    printf("El valor del shlvl es %s\n", shlvl);
    return(0);
}
