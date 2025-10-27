/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:17:58 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:34:49 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Frees the memory allocated for the shell's environment list.
 * 
 * This function iterates through the `own_env` linked list in the `t_msl`
 * structure and frees the memory allocated for each node, including the
 * `id` and `value` fields. Once all nodes are freed, the `own_env` pointer
 * is set to `NULL`.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
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

/**
 * @brief Frees all memory allocated for the main shell structure.
 * 
 * This function releases all resources associated with the `t_msl` structure,
 * including the environment list, tokens, lexer, parsing utilities, system
 * data, and built-in commands. It ensures that all dynamically allocated
 * memory is properly freed to prevent memory leaks.
 * 
 * - Calls `free_own_env` to free the environment list.
 * - Calls `free_tockens` to free the token list.
 * - Frees the `clean_line`, `lexer`, `parsing_utils`, `sys`, and `builts` fields.
 * - Sets the `t_msl` pointer to `NULL` after freeing.
 * 
 * @param msl Pointer to the pointer of the main structure of the shell.
 */
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

/**
 * @brief Frees the memory allocated for the system-related data.
 * 
 * This function releases all resources associated with the `t_system`
 * structure, including the user, host, home directory, global path, current
 * working directory, and shell prompt components. Once all fields are freed,
 * the `t_system` pointer is set to `NULL`.
 * 
 * - Frees the `host`, `user`, `home`, `g_path`, `pwd`, `ps1_hostuser`,
 *   `ps1_path`, and `ps1` fields.
 * - Sets the `t_system` pointer to `NULL` after freeing.
 * 
 * @param sys Pointer to the pointer of the system structure.
 */
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
