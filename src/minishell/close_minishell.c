

#include "../../inc/minishell.h"

//Funcion para borrar la estrcutura de control de la minishell en caso de que hagamos 
//exit o control+D. (Se tiene que manejar el caso de que cerremos la ventana??)

void	free_sys(t_system *sys);

void free_own_env(t_msl *msl)
{
	t_env *env;
	t_env *tmp;

	if (!msl || !msl->own_env)
		return ;
	env = msl->own_env;
	while (env)
	{
		if (env->id)
			free(env->id);
		if (env->value)
			free(env->value);
		tmp = env;
		env = env->next;
		free(tmp);
	}
	msl->own_env = NULL;
}

void free_msl(t_msl **msl)
{
	if (!msl || !*msl)
		return ;
	free_own_env(*msl);
	free_tockens(*msl);
	if ((*msl)->clean_line)
		free((*msl)->clean_line);
	if ((*msl)->parsing_utils)
		free((*msl)->parsing_utils);
	if ((*msl)->lexer)
		free_lexer(*msl, 1);
	if ((*msl)->sys)
		free_sys((*msl)->sys);
	free(*msl);
	*msl = NULL;
}

void	free_sys(t_system *sys)
{
	if (!sys)
		return ;
	if (sys->host)
		free(sys->host);
	if (sys->user)
		free(sys->user);
	if (sys->home)
		free(sys->home);
	if (sys->ps1)
		free(sys->ps1);
	free(sys);
}
