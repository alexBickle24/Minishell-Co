/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_host_user.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:50:04 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:50:42 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Retrieves the hostname using multiple fallback methods.
 * 
 * This function attempts to retrieve the system's hostname using a series
 * of fallback methods:
 * - First, it tries to read the hostname from `/etc/hostname` using `get_hostnamedir`.
 * - If that fails, it searches for the `hostname` command using `which_cmd`
 *   and executes it to retrieve the hostname.
 * - If both methods fail, the hostname is set to `NULL`.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param target Pointer to the string where the hostname will be stored.
 */
void	hostname_fallbacks(t_msl *msl, char **target)
{
	int		fallback;
	char	*abs_path;

	fallback = 0;
	if (fallback == 0)
		fallback += get_hostnamedir(&(msl->sys->host));
	if (fallback == 1)
	{
		abs_path = which_cmd(msl, "/usr/bin/which", "hostname");
		if (abs_path)
			fallback += get_infcmds(msl, &(msl->sys->host), abs_path, NULL);
		else
			fallback++;
	}
	if (fallback == 2)
		*target = NULL;
}

/**
 * @brief Reads the hostname from the `/etc/hostname` file.
 * 
 * This function attempts to open the `/etc/hostname` file and read the
 * hostname from it. If successful, the hostname is stored in the `target`
 * pointer. If the file cannot be opened or read, the function returns an error.
 * 
 * - Opens `/etc/hostname` in read-only mode.
 * - Reads the hostname using `get_next_line`.
 * - Removes the trailing newline character from the hostname.
 * - Closes the file descriptor after reading.
 * 
 * @param target Pointer to the string where the hostname will be stored.
 * @return `0` if the hostname is successfully retrieved, `1` otherwise.
 */
int	get_hostnamedir(char **target)
{
	int		fd;
	char	*hostname;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return (1);
	else
	{
		hostname = get_next_line(fd);
		if (hostname == NULL)
			return (1);
		hostname[ft_strlen(hostname) - 1] = '\0';
		*target = hostname;
		close (fd);
		return (0);
	}
}

/**
 * @brief Retrieves the username using multiple fallback methods.
 * 
 * This function attempts to retrieve the current user's username using a
 * series of fallback methods:
 * - First, it searches for the `whoami` command using `which_cmd` and executes
 *   it to retrieve the username.
 * - If that fails, it searches for the `id` command and executes it with the
 *   `-un` flag to retrieve the username.
 * - If both methods fail, it calls `user_fallbacks2` to check the `USER`
 *   environment variable or set the username to `NULL`.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
void	user_fallbacks(t_msl *msl)
{
	int		fallback;
	char	*abs_path;

	fallback = 0;
	abs_path = NULL;
	if (fallback == 0)
	{
		abs_path = which_cmd(msl, "/usr/bin/which", "whoami");
		if (abs_path)
			fallback += get_infcmds(msl, &(msl->sys->user), abs_path, NULL);
		else
			fallback++;
	}
	if (fallback == 1)
	{
		abs_path = which_cmd(msl, "/usr/bin/which", "id");
		if (abs_path)
			fallback += get_infcmds(msl, &(msl->sys->user), abs_path, "-un");
		else
			fallback ++;
	}
	if (fallback == 2)
		user_fallbacks2(msl, fallback);
	if (abs_path != NULL)
		free(abs_path);
}

/**
 * @brief Retrieves the username from the `USER` environment variable or sets it to `NULL`.
 * 
 * This function is called as a final fallback when other methods to retrieve
 * the username have failed. It checks the `USER` environment variable:
 * - If the `USER` variable exists, its value is duplicated and stored as the username.
 * - If the `USER` variable does not exist, the username is set to `NULL`.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param fallback The current fallback level (should be `2` when called).
 */
void	user_fallbacks2(t_msl *msl, int fallback)
{
	t_env	*node;

	if (fallback == 2)
	{
		node = search_id_node(msl, "USER");
		if (node == NULL)
			fallback++;
		else
			msl->sys->user = ft_strdup(node->value);
	}
	if (fallback == 3)
		msl->sys->user = NULL;
}
