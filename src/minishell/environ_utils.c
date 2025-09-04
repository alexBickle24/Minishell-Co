

#include "../../inc/minishell.h"

//Para reservar memeria para las key de una fila de la tabla (key=value)
char *ft_get_env_id(char *env_line)
{
    char *id;
    int i;
    int j;

    i = 0;
    while (env_line[i] && env_line[i] != '=')
        i++;
    id = (char *)malloc(sizeof(char) * (i + 1));
    if (!id)
        return (NULL);
    j = 0;
    while (j < i)
    {
        id[j] = env_line[j]; 
        j++;
    }
    id[j] = '\0';
    return (id);
}

/* Funcion para extraer valores de variables de entorno
 * Luego estas variables las modificaremos segun lo necesitemos
 * Ejemplo: PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin
 * ft_get_env(env, PATH)
 *     /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin
 */

char *ft_get_env_value(char **env, char *id)
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
        if (ft_strncmp(env[i], id, ft_strlen_c(id)) == 0 && env[i][ft_strlen_c(id)] == '=')
        {
            j = ft_strlen_c(id) + 1;
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

//Para buscar el nodo que contiene un id concreto
t_env *search_id_node(t_msl *msl, char *id)
{
    t_env *current;
    
    if (!msl)
        return (NULL);
    current = msl->own_env;
    while(current)
    {
        if (ft_strncmp(current->id, id, ft_strlen(id) + 1) == 0)
            return (current);
        current = current->next;
    }
    return (NULL);
}

//Para añadir nodos a la lista de own, enviroment
t_env *list_new_ownenv(char *id, char *value)
{
    t_env *node;
    
    node = (t_env *)malloc(sizeof(t_env));
    if (node == NULL)
        return (NULL);
    node->id = id;
    node->value = value;
    node->next = NULL;
    return (node);
}

void  list_add_back_env(t_env *node, t_env **env)
{
    t_env *current;

    if (!env)
        return ;   
    if (!*env)
    {
        *env = node;
        return ;
    }
    else
    {
        current = *env;
        while(current->next)
            current = current->next;
        current->next = node;
    }
}
