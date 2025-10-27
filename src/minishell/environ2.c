/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 23:19:24 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 15:08:15 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sets the `LESSOPEN` environment variable if it is not already defined.
 * 
 * This function ensures that the `LESSOPEN` environment variable is set to
 * its default value if it is not already present in the shell's environment.
 * The `LESSOPEN` variable is used by the `less` command to preprocess files
 * before displaying them.
 * 
 * - If `LESSOPEN` is not found in the environment, it is created with the
 *   default value: `"| /usr/bin/lesspipe %s"`.
 * - Allocates memory for the variable and its value, and adds it to the
 *   environment list.
 * - If memory allocation fails, the function cleans up and returns.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
void	set_lessopen(t_msl *msl)
{
	t_env	*l_open;
	char	*l_open_val;

	if (!msl)
		return ;
	l_open = search_id_node(msl, "LESSOPEN");
	if (l_open == NULL)
	{
		l_open_val = ft_strdup("| /usr/bin/lesspipe %s");
		l_open = list_new_ownenv(ft_strdup("LESSOPEN"), l_open_val);
		if (!l_open)
		{
			free(l_open_val);
			return ;
		}
		list_addback_env(l_open, &(msl->own_env));
	}
}
/**
 * @brief Sets the `LESSCLOSE` environment variable if it is not already defined.
 * 
 * This function ensures that the `LESSCLOSE` environment variable is set to
 * its default value if it is not already present in the shell's environment.
 * The `LESSCLOSE` variable is used by the `less` command to clean up temporary
 * files after processing.
 * 
 * - If `LESSCLOSE` is not found in the environment, it is created with the
 *   default value: `"/usr/bin/lesspipe %s %s"`.
 * - Allocates memory for the variable and its value, and adds it to the
 *   environment list.
 * - If memory allocation fails, the function cleans up and returns.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
void	set_lessclose(t_msl *msl)
{
	t_env	*l_close;
	char	*l_close_val;

	if (!msl)
		return ;
	l_close = search_id_node(msl, "LESSCLOSE");
	if (l_close == NULL)
	{
		l_close_val = ft_strdup("/usr/bin/lesspipe %s %s");
		l_close = list_new_ownenv(ft_strdup("LESSCLOSE"), l_close_val);
		if (!l_close)
		{
			free(l_close_val);
			return ;
		}
		list_addback_env(l_close, &(msl->own_env));
	}
}

/**
 * @brief Sets or updates the `_` environment variable to the value of `PATH`.
 * 
 * This function ensures that the `_` environment variable, which stores the
 * last argument of the last executed command, is initialized to the value of
 * the `PATH` environment variable during shell initialization.
 * 
 * - Searches for the `PATH` variable in the environment. If it is not found,
 *   the function returns without making changes.
 * - If `_` is already defined, its value is updated to match `PATH`.
 * - If `_` is not defined, it is created with the value of `PATH`.
 * - Allocates memory for the variable and its value, and adds it to the
 *   environment list.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
void	set_last_arg_init(t_msl *msl)
{
	t_env	*last;
	t_env	*path;

	path = search_id_node(msl, "PATH");
	if (!path)
		return ;
	last = search_id_node(msl, "_");
	if (last)
	{
		free(last->value);
		last->value = ft_strdup(path->value);
	}
	else
	{
		last = list_new_ownenv(ft_strdup("_"), ft_strdup(path->value));
		if (!last)
			return ;
		list_addback_env(last, &(msl->own_env));
	}
}
