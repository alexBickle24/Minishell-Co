/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:23:48 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/20 20:24:56 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		init_builts(*msl);
		msl[0]->parsing_utils = init_parsing(*msl);
		msl[0]->msl_pid = (pid_t)ft_getpid();
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
		hostname_fallbacks(msl, &(msl->sys->host));
		get_home(msl, &(msl->sys->home));
		get_global_path(&(msl->sys->g_path));
		msl->sys->ps1_hostuser = set_ps1_hostuser(msl->sys);
	}
}

void	set_env_default_values(t_msl *msl)
{
	check_home_message(msl);
	set_shlvl(msl);
	set_pwd(msl);
	set_path(msl);
	set_lessclose(msl);
	set_lessopen(msl);
}

void	init_builts(t_msl *msl)
{
	if (msl->builts == NULL)
		msl->builts = (t_builts*)ft_calloc(sizeof(t_builts), 1);
}

void	set_path(t_msl *msl)
{
	t_env	*path_node;
	char	*glo_path;

	glo_path = msl->sys->g_path;
	path_node = search_id_node(msl, "PATH");
	if (path_node == NULL)
	{
		path_node = list_new_ownenv(ft_strdup("PATH"), ft_strdup(glo_path));
		if (!path_node)
			return ;
		list_addback_env(path_node, &(msl->own_env));
	}
}
