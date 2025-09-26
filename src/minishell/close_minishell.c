/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:17:58 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/26 23:18:27 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_own_env(t_msl *msl)
{
	t_env	*env;
	t_env	*tmp;

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

void	free_msl(t_msl **msl)
{
	if (!msl || !*msl)
		return ;
	free_own_env(*msl);
	free_tockens(*msl);
	if ((*msl)->clean_line)
		free((*msl)->clean_line);
	if ((*msl)->lexer)
		free_lexer(*msl, 1);
	if ((*msl)->parsing_utils)
		free((*msl)->parsing_utils);
	if ((*msl)->sys)
		free_sys(&((*msl)->sys));
	if ((*msl)->builts)
		free((*msl)->builts);
	free(*msl);
	*msl = NULL;
}

void	free_sys(t_system **sys)
{
	if (!sys || !*sys)
		return ;
	if ((*sys)->host)
		free((*sys)->host);
	if ((*sys)->user)
		free((*sys)->user);
	if ((*sys)->home)
		free((*sys)->home);
	if ((*sys)->g_path)
		free((*sys)->g_path);
	if ((*sys)->pwd)
		free((*sys)->pwd);
	if ((*sys)->ps1_hostuser)
		free((*sys)->ps1_hostuser);
	if ((*sys)->ps1_path)
		free((*sys)->ps1_path);
	if ((*sys)->ps1)
		free((*sys)->ps1);
	free(*sys);
	*sys = NULL;
}
