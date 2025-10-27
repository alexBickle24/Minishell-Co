/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_ps1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:08:20 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:58:35 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Creates the "user@host" portion of the shell prompt (`PS1`).
 * 
 * This function constructs the "user@host" string for the shell prompt by
 * concatenating the username and hostname with formatting and color codes.
 * 
 * - Combines the username (`sys->user`) and hostname (`sys->host`) with an `@` separator.
 * - Adds a green color code (`C_GREEN`) and a reset code (`C_RESET`) for formatting.
 * - Frees intermediate strings to avoid memory leaks.
 * 
 * @param sys Pointer to the `t_system` structure containing user and host information.
 * @return A dynamically allocated string representing the "user@host" portion of the prompt,
 *         or `NULL` if memory allocation fails.
 */
char	*set_ps1_hostuser(t_system *sys)
{
	char	*tmp;
	char	*host_user;
	char	*final;

	if (!sys || !sys->host || !sys->user)
		return (NULL);
	tmp = ft_strjoin(sys->user, "@");
	if (!tmp)
		return (NULL);
	host_user = ft_strjoin(tmp, sys->host);
	free(tmp);
	if (!host_user)
		return (NULL);
	tmp = ft_strjoin(C_GREEN, host_user);
	free(host_user);
	if (!tmp)
		return (NULL);
	final = ft_strjoin(tmp, C_RESET ":");
	free(tmp);
	return (final);
}

/**
 * @brief Creates the current working directory portion of the shell prompt (`PS1`).
 * 
 * This function constructs the path portion of the shell prompt by retrieving
 * the current working directory (`PWD`) and formatting it with color codes.
 * If the current directory is inside the user's home directory, it replaces
 * the home directory path with `~`.
 * 
 * - Retrieves the current working directory from `msl->sys->pwd`.
 * - Checks if the `PWD` environment variable matches the current directory.
 * - Calls `get_display_path` to handle home directory replacement with `~`.
 * - Adds a blue color code (`C_BLUE`) and a reset code (`C_RESET`) for formatting.
 * - Frees intermediate strings to avoid memory leaks.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @return A dynamically allocated string representing the path portion of the prompt,
 *         or `NULL` if memory allocation fails.
 */
char	*set_ps1_path(t_msl *msl)
{
	char	*cwd;
	char	*display_path;
	char	*color_path;
	t_env	*pwd_node;

	cwd = msl->sys->pwd;
	pwd_node = search_id_node(msl, "PWD");
	if (!pwd_node || !ft_strncmp(pwd_node->value, cwd, ft_strlen(cwd) + 1))
		display_path = get_display_path(msl, cwd);
	else
		display_path = ft_strdup(pwd_node->value);
	if (!display_path)
		return (NULL);
	color_path = ft_strjoin(C_BLUE, display_path);
	free(display_path);
	if (!color_path)
		return (NULL);
	display_path = ft_strjoin(color_path, C_RESET);
	free(color_path);
	return (display_path);
}

/**
 * @brief Formats the current working directory for display in the shell prompt.
 * 
 * This function checks if the current working directory (`cwd`) is inside the
 * user's home directory. If it is, it replaces the home directory path with `~`.
 * Otherwise, it returns the full path as is.
 * 
 * - Searches for the `HOME` environment variable using `search_id_node`.
 * - If the current directory starts with the home directory path, replaces it with `~`.
 * - If the home directory is not found or does not match, returns the full path.
 * - Frees intermediate strings to avoid memory leaks.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param cwd The current working directory.
 * @return A dynamically allocated string representing the formatted path,
 *         or `NULL` if memory allocation fails.
 */
char	*get_display_path(t_msl *msl, char *cwd)
{
	t_env	*home_node;
	char	control;
	char	*find;
	char	*display_path;

	home_node = search_id_node(msl, "HOME");
	control = 0;
	if (home_node)
	{
		find = ft_strnstr(cwd, home_node->value, ft_strlen(home_node->value));
		if (find)
		{
			display_path = ft_strjoin("~", find + ft_strlen(home_node->value));
			if (!display_path)
				return (free(cwd), NULL);
		}
		else
			control = 1;
	}
	if (!home_node || control == 1)
		display_path = ft_strdup(cwd);
	return (display_path);
}

/**
 * @brief Sets the full shell prompt (`PS1`).
 * 
 * This function constructs the full shell prompt by combining the "user@host"
 * portion and the current working directory portion. It also appends a `$`
 * or `#` symbol depending on the shell's state.
 * 
 * - Calls `set_ps1_path` to generate the path portion of the prompt.
 * - Combines the "user@host" portion (`sys->ps1_hostuser`) and the path portion.
 * - Appends the `BOOSTER` symbol (`$` or `#`) to the prompt.
 * - Frees any previously allocated prompt strings to avoid memory leaks.
 * - If memory allocation fails, sets a default prompt (`minishell$`).
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param sys Pointer to the `t_system` structure containing prompt components.
 */
void	set_ps1(t_msl *msl, t_system *sys)
{
	char	*tmp;
	char	*dollar;

	if (msl->sys->ps1_path)
		free(msl->sys->ps1_path);
	if (sys->ps1)
		free(sys->ps1);
	msl->sys->ps1_path = set_ps1_path(msl);
	if (!sys->ps1_hostuser || !sys->ps1_path)
		tmp = ft_strdup("minishell");
	else
		tmp = ft_strjoin(sys->ps1_hostuser, sys->ps1_path);
	dollar = ft_strjoin(tmp, BOOSTER);
	free(tmp);
	if (dollar)
		sys->ps1 = dollar;
	else
		sys->ps1 = ft_strdup("\001\033[0;32m\002minishell\001\033[0m\002$");
}
