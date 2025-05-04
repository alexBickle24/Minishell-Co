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

/* Funcion para extraer valores de variables de entorno
 * Luego estas variables las modificaremos segun lo necesitemos
 * Ejemplo: PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin
 * ft_get_env(env, PATH)
 *     /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin
 */
char *ft_get_env(char **env, char *id)
{
    int i;
    int j;
    int k;
    int len;
    char *value;

    if (!env || !id)
        return NULL;
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], id, ft_strlen(id)) == 0 && env[i][ft_strlen(id)] == '=')
        {
            j = ft_strlen(id) + 1;
            len = ft_strlen(env[i]) - j;
            value = malloc(sizeof(char) * (len + 1));
            if (!value)
                return NULL;
            k = 0;
            while (env[i][j])
                value[k++] = env[i][j++];
            value[k] = '\0';
            return value;
        }
        i++;
    }
    return NULL;
}