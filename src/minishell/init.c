

#include "../../inc/minishell.h"

void minishell_init(t_msl **msl, char **env, char **argv)
{
    if (msl != NULL)
    {
       *msl = (t_msl *)ft_calloc(sizeof(t_msl), 1);
        if (!*msl)
            return ;
        msl[0]->parsing_utils = init_parsing(*msl);//la utils de parseo
        msl[0]->msl_pid = (pid_t)ft_getpid();//el pid del proceso
        env_init(*msl, env, argv);//pasa la tabla de env a formato lista para no tener que hacer reallocs
        print_msl(msl[0]);
        signal_init();
    }
}
