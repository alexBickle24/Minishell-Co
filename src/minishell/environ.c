/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:09:19 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 15:05:25 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Converts an environment variable table to a linked list.
 * 
 * This function iterates through the environment variable table (`env_tb`)
 * and creates a linked list of `t_env` nodes. Each node represents an
 * environment variable in the format `key=value`.
 * 
 * - If the `msl->own_env` list is empty, it initializes the list.
 * - Calls `search_creat_envnode` to create a new node for each environment variable.
 * - Links the newly created nodes to the end of the list.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param env_tb Array of environment variables in `key=value` format.
 */

void	ft_env_table_to_list(t_msl *msl, char **env_tb)
{
	t_env	*tmp;
	t_env	*last_node;

	if (!msl || !env_tb)
		return ;
	last_node = msl->own_env;
	if (msl->own_env == NULL)
	{
		while (*env_tb)
		{
			tmp = search_creat_envnode(msl, env_tb);
			if (!tmp)
				return ;
			if (!(msl->own_env))
				msl->own_env = tmp;
			else
				last_node->next = tmp;
			last_node = tmp;
			env_tb++;
		}
	}
}

/**
 * @brief Searches for or creates a new environment variable node.
 * 
 * This function extracts the `key` and `value` from an environment variable
 * string and creates a new `t_env` node. If memory allocation fails, it
 * frees the allocated resources and the entire environment list.
 * 
 * - Extracts the `key` using `ft_get_env_id`.
 * - Extracts the `value` using `ft_get_env_value`.
 * - Creates a new node using `list_new_ownenv`.
 * - If any step fails, it frees the allocated memory and the environment list.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param env_tb Pointer to the current environment variable string.
 * @return A pointer to the newly created `t_env` node, or `NULL` on failure.
 */

t_env	*search_creat_envnode(t_msl *msl, char **env_tb)
{
	char	*id;
	char	*value;
	t_env	*tmp;

	id = ft_get_env_id(*env_tb);
	value = ft_get_env_value(env_tb, id);
	if (!value)
	{
		free(id);
		free(value);
		free_own_env(msl);
		return (NULL);
	}
	tmp = list_new_ownenv(id, value);
	if (!tmp)
	{
		free(id);
		free(value);
		free_own_env(msl);
		return (NULL);
	}
	return (tmp);
}

/**
 * @brief Sets or updates the `SHLVL` environment variable.
 * 
 * This function manages the `SHLVL` variable, which tracks the shell level.
 * If `SHLVL` does not exist, it creates it with a value of `1`. If it exists,
 * the value is incremented. Special cases:
 * - If `SHLVL` is greater than or equal to 999, a warning is displayed, and
 *   the value is reset to `1`.
 * - If `SHLVL` is negative, it is reset to `0`.
 * 
 * @param msl Pointer to the main structure of the shell.
 */

void	set_shlvl(t_msl *msl)
{
	t_env	*shlvl_node;
	int		shlvl_num;

	if (!msl)
		return ;
	shlvl_node = search_id_node(msl, "SHLVL");
	if (shlvl_node == NULL)
	{
		shlvl_node = list_new_ownenv(ft_strdup("SHLVL"), ft_itoa(1));
		list_addback_env(shlvl_node, &(msl->own_env));
		return ;
	}
	shlvl_num = ft_atoi(shlvl_node->value);
	free(shlvl_node->value);
	if (shlvl_num >= 999)
	{
		ft_shlvl_warning(++shlvl_num);
		shlvl_node->value = ft_strdup("1");
	}
	else if (shlvl_num < 0)
		shlvl_node->value = ft_strdup("0");
	else
		shlvl_node->value = ft_itoa(++shlvl_num);
}

/**
 * @brief Checks the `HOME` environment variable and displays a warning if invalid.
 * 
 * This function verifies the `HOME` environment variable. If `HOME` is not
 * set or its value does not match the shell's internal `home` directory, it
 * displays an error message.
 * 
 * - If the shell is in non-interactive mode (`msl->mode == 1`), the function
 *   does nothing.
 * - Searches for the `HOME` variable using `search_id_node`.
 * - Compares the `HOME` value with the shell's internal `home` directory.
 * - Displays an error message if `HOME` is missing or invalid.
 * 
 * @param msl Pointer to the main structure of the shell.
 */

void	check_home_message(t_msl *msl)
{
	t_env	*node;

	if (msl->mode == 1)
		return ;
	node = search_id_node(msl, "HOME");
	if (node == NULL)
	{
		ft_error_home();
		return ;
	}
	if (ft_strncmp(node->value, msl->sys->home, ft_strlen(node->value) + 1))
	{
		ft_error_home();
		return ;
	}
}

/**
 * @brief Sets or updates the `PWD` environment variable.
 * 
 * This function ensures that the `PWD` environment variable is set to the
 * current working directory. If `PWD` does not exist, it creates it. If it
 * exists, its value is updated.
 * 
 * - Retrieves the current working directory using `getcwd`.
 * - If `getcwd` fails, sets `PWD` to `"."`.
 * - Searches for the `PWD` variable using `search_id_node`.
 * - Updates the `PWD` value if it exists, or creates a new `PWD` variable if not.
 * 
 * This function is particularly useful if the shell is initialized from a
 * directory whose parent directory has been deleted.
 * 
 * @param msl Pointer to the main structure of the shell.
 */

void	set_pwd(t_msl *msl)
{
	t_env	*env_node;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		pwd = ft_strdup(".");
	env_node = search_id_node(msl, "PWD");
	if (env_node)
	{
		if (env_node->value)
		{
			free(env_node->value);
			env_node->value = pwd;
		}
	}
	else
	{
		env_node = list_new_ownenv(ft_strdup("PWD"), pwd);
		if (!env_node)
		{
			free(pwd);
			return ;
		}
		list_addback_env(env_node, &(msl->own_env));
	}
}
