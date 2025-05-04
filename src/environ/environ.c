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

// Funcion para extraer cada variable de entorno
/*
 * 1. Las variables van desde el principio hasta \n
 * 2. Los nombres van desde el principio hasta =
 * 3. Los valores pueden ser varios, separados con ;
 * 
 * ¿Como los organizo?
 * [PATH] - [/usr/local/sbin, /usr/local/bin, ...]
 * [GDMSESSION] - [ubuntu]
 * 
 *      - La estructura debe ser un nombre de valor
 *      - Y un valor en modo lista (puede ser de 1 o mas elementos)
 */
void    ft_get_env(char **env, char *id)
{
    int i;

    i = -1;
    while (env[++i])
    {
        if (ft_strncmp(env[i], id, ft_strlen(id)) == 0)
            printf("result: %s", env[i]);
    }
    //return (id);
}