#include "../../inc/minishell.h"

void minishell_init(t_msl **msl, char **env)
{
	if (msl != NULL)
	{
		*msl = (t_msl *)ft_calloc(sizeof(t_msl), 1);
		if (*msl == NULL)
			return ;
		ft_env_table_to_list(*msl, env);
		init_system(*msl);
		set_env_default_values(*msl);
		// print_own_env(msl[0]->own_env);
		init_builts(*msl);
		msl[0]->parsing_utils = init_parsing(*msl);//la utils de parseo
		msl[0]->msl_pid = (pid_t)ft_getpid();//el pid del procesos
		signal_init();
	}
}

void	init_system(t_msl *msl)
{
	if (msl == NULL)
		return ;
	if (msl->sys == NULL)
	{
		msl->sys = (t_system *)ft_calloc(sizeof(t_system), 1);
		if (msl->sys == NULL)
			return ;
		user_fallbacks(msl);
		// printf("el valor del hostname A es %s\n", msl->sys->user);
		hostname_fallbacks(msl, &(msl->sys->host));
		// printf("el valor del hostname A es %s\n", msl->sys->host);
		get_home(msl, &(msl->sys->home));
		// printf("el valor del home A es %s\n", msl->sys->home);
		get_global_path(&(msl->sys->g_path));
		// printf("el valor del hostname A es %s\n", msl->sys->g_path);
		msl->sys->ps1_hostuser = set_ps1_hostuser(msl->sys);
		// printf("el valor de ps1 hostuser es %s \n", msl->sys->ps1_hostuser);
		// set_ps1(msl, msl->sys);
		// printf("el valor de ps1 path es %s \n", msl->sys->ps1_path);
		// printf("el valor de PS1_prompt es %s \n", msl->sys->ps1);
	}
}

void	set_env_default_values(t_msl *msl)
{
	set_shlvl(msl);
	check_home_message(msl);
	set_pwd(msl);
	set_path(msl);
}
void	init_builts(t_msl *msl)
{
	if (msl->builts == NULL)
		msl->builts = (t_builts*)ft_calloc(sizeof(t_builts), 1);
}

void	set_path(t_msl *msl)
{
	t_env	*path_node;
	char	*g_path;

	g_path = msl->sys->g_path;
	path_node = search_id_node(msl, "PATH");
	if (path_node == NULL)//OK
	{
		path_node = list_new_ownenv(ft_strdup("PATH"), ft_strdup(g_path));
		if (!path_node)
			return ;
		list_addback_env(path_node, &(msl->own_env));
	}
}
