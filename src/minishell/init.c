

#include "../../inc/minishell.h"

void minishell_init(t_msl **msl, char **env, char **argv)
{
    if (msl != NULL)
    {
       *msl = (t_msl *)malloc(sizeof(t_msl) * 1);
        if (!*msl)
            return ;
        msl[0]->own_env = NULL;
        msl[0]->exit_status = 0; //valor por defecto
        msl[0]->clean_line = NULL;//la linea limpia (trimeada)
        msl[0]->msl_pid = ft_getpid();//el pid del proceso
        msl[0]->total_tockens = 0;//los tockens creados
        msl[0]->tocken = NULL;//la lista de tockens
        env_init(*msl, env, argv);//pasa la tabla de env a formato lista para no tener que hacer reallocs
        print_own_env(msl[0]->own_env);
        signal_init();
    }
}

//Aqui ira interpreter modde cuando este creada
