#include "minishell.h"





void env_init(t_msl *msl, char **inherid_env, char **argv)
{
    (void)argv;
    ft_env_table_to_list(msl, inherid_env);
    // set_env_default_values(msl, argv);
    set_shlvl(msl);
}

void ft_env_table_to_list(t_msl *msl, char **env_tb)
{
    t_env *tmp;
    t_env *last_node;
    char   *id;
    char    *value;

    if (!msl || !env_tb)
        return ;
    last_node = msl->own_env;
    if (msl->own_env == NULL)
    {
        while (*env_tb)
        {
            id = ft_get_env_id(*env_tb);
            value = ft_get_env_value(env_tb, id);
            if (!value)
            {
                free(id);
                free(value);
                free_own_env(msl);
                return ;
            }
            tmp = list_new_ownenv(id, value);
            if (!tmp)
            {
                free_own_env(msl);
                return ;
            }
            if (!(msl->own_env))
                msl->own_env = tmp;
            else
                last_node->next = tmp;
            last_node = tmp;
            env_tb++;
        }
    }
}

/*
    //Funcion para recorrer los valores del env heredado y poner los default values
    //SHELL: Libre albedrio: ruta relativa o absoluta a minishell argv[0], shell asignado a usuario en etc passwd, 
    //PWD: Si no exite-> Se crea
    //PATH: No se puede cambiar con export pero si borrar. En /etc/environment
    //HOME: Si no existe-> Se crea (complicado)
    //SHLVL: Si no existe-> Se crea

    //El resto de valores creo que no son obligatorios...
*/

void set_env_default_values(t_msl *msl, char **argv)
{
    if (!msl || !argv[0])
        return ;
    list_add_back_env(list_new_ownenv("SHELL", get_minipath(argv)), &msl->own_env);
    if (!search_id_node(msl, "PWD"))
        list_add_back_env(list_new_ownenv("PWD", getcwd(NULL, 0)), &msl->own_env);
    if (!search_id_node(msl, "PATH"))
    {
        
    }
    
    
}

//Aumenta el valor de SHLVL
void set_shlvl(t_msl *msl)
{
    t_env *shlvl_node;
    int    shlvl_num;

    if (!msl || !msl->own_env)
        return ;
    shlvl_node = search_id_node(msl, "SHLVL");
    if (shlvl_node == NULL)//estopuede que haya que cambiarlo
        return ;
    shlvl_num = ft_atoi(shlvl_node->value);
    free(shlvl_node->value);
    if (shlvl_num >= 999)
    {
        ft_shlvl_warning(shlvl_node->value);
        shlvl_node->value = ft_strdup("1");
    }
    else if (shlvl_num < 0)
        shlvl_node->value = ft_strdup("0");
    else
        shlvl_node->value = ft_itoa(shlvl_num++);
}

//cambiar argv por trimname
char *get_minipath(char **argv)
{
    char *cwd;
    char *tmp;
    char *shell_path;

    if (!argv || argv[0] == NULL)
        return (NULL);
    cwd = getcwd(NULL, 0);
    if (!cwd)
        return (NULL);
    tmp = ft_strjoin(cwd, "/");
    if (!tmp)
        return (free(cwd), NULL);
    shell_path = ft_strjoin(tmp, argv[0]);
    if (!shell_path)
        return (free(cwd), free(tmp), NULL);
    return (shell_path);
}
