

#include "../../inc/minishell.h"


//falta adaptarlas a norminette
char **ft_env_to_table(t_env *env)
{
    t_env *current;
    int lines;
    char *tmp;
    char **table;

    //contar las lineas
    current = env;
    if (!env)
        return (NULL);
    lines = 0;
    while (current)
    {
        current = current->next;
        lines++;
    }
    table = (char **)malloc(sizeof(char *) * (lines + 1));
    if (!table)
        return (NULL);
    //Hacer strjoin de key y value de cada nodo y añadir a la tabla (filas)
    current = env;
    lines = 0;
    while (current)
    {
        tmp = ft_strjoin(current->id, "=");
        if (!tmp)
            return (NULL);
        table[lines] = ft_strjoin(tmp, current->value);
        if (!table[lines])
            return(NULL);
        free(tmp);
        current = current->next;
        lines++;
    }
    table[lines] = NULL;
    return(table);
}


char **ft_pcmds_to_table(t_pcmds *pcmds)
{
    t_pcmds *current;
    int lines;
    char **table;

    current = pcmds;
    if (!pcmds)
        return (NULL);
    lines = 0;
    while (current)
    {
        current = current->next;
        lines++;
    }
    table = (char **)malloc(sizeof(char *) * (lines + 1));
    if (!table)
        return (NULL);
    current = pcmds;
    lines = 0;
    while (current)
    {
        table[lines] = ft_strdup(current->cmd);
        if (!table[lines])
            return (NULL);
        current = current->next;
        lines++;
    }
    table[lines] = NULL;
    return(table);
}

