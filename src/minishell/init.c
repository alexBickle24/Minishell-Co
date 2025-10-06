/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:23:48 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/05 14:46:59 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	minishell_init(t_msl **msl, char **env, char mode)
{
	if (msl != NULL)
	{
		*msl = (t_msl *)ft_calloc(sizeof(t_msl), 1);
		if (*msl == NULL)
			return ;
		ft_env_table_to_list(*msl, env);
		(*msl)->mode = mode;
		init_system(*msl);
		set_env_default_values(*msl);
		if ((*msl)->builts == NULL)
			(*msl)->builts = (t_builts *)ft_calloc(sizeof(t_builts), 1);
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
		init_pwd(msl);
		msl->sys->ps1_hostuser = set_ps1_hostuser(msl->sys);
	}
}

void	set_env_default_values(t_msl *msl)
{
	check_home_message(msl);
	set_shlvl(msl);
	set_pwd(msl);
	set_path(msl);
	set_last_arg_init(msl);
	set_lessclose(msl);
	set_lessopen(msl);
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

void	init_pwd(t_msl *msl)
{
	msl->sys->pwd = getcwd(NULL, 0);
	if (msl->sys->pwd == NULL)
	{
		ft_putstr_fd("shell-init: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		msl->sys->pwd = ft_strdup(".");
	}
}
