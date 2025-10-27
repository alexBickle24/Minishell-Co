/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:46:29 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:13:58 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Evaluates errors in the command of a token and resolves its path.
 * 
 * This function checks for errors in the command associated with a token
 * and resolves its executable path. It handles direct paths, searches for
 * the command in the `PATH` environment variable, and sets error codes
 * if the command is invalid or inaccessible.
 * 
 * - Converts the `pcmds` list to a command table (`cmd_tb`) if not already set.
 * - Converts the environment list to a table (`env_tb`) if not already set.
 * - Handles direct paths using `handle_direct_path`.
 * - Searches for the command in the `PATH` variable using `check_path`.
 * - Sets error codes:
 *   - `1`: Command exists but is not executable.
 *   - `2`: Command not found.
 *   - `3`: Command is a directory.
 * 
 * @param c_tocken Pointer to the current token being evaluated.
 * @param msl Pointer to the main structure of the shell.
 */
void	evaluate_tocken_cmds_errors(t_tocken *c_tocken, t_msl *msl)
{
	char	*path;

	if (!c_tocken->pcmds)
		return ;
	if (!c_tocken->cmd_tb)
		c_tocken->cmd_tb = ft_pcmds_to_table(c_tocken->pcmds);
	if (!c_tocken->env_tb)
		c_tocken->env_tb = ft_env_to_table(msl->own_env);
	if (handle_direct_path(c_tocken))
		return ;
	path = check_path(c_tocken->cmd_tb[0], c_tocken->env_tb);
	if (!path)
	{
		c_tocken->error_cmd = 2;
		return ;
	}
	free(c_tocken->pcmds->cmd);
	c_tocken->pcmds->cmd = path;
	if (access(path, X_OK) == -1)
		c_tocken->error_cmd = 1;
}

/**
 * @brief Handles direct paths for commands.
 * 
 * This function checks if the command is provided as a direct path (contains `/`).
 * A direct path can be a relative or absolute path to the executable. The shell
 * does not allow creating files with `/` in their name, so any command containing
 * `/` is treated as a path.
 * 
 * - If the path exists and is a directory, sets `error_cmd = 3`.
 * - If the path exists but is not executable, sets `error_cmd = 1`.
 * 
 * @param c_tocken Pointer to the current token being evaluated.
 * @return `1` if the command is a direct path, `0` otherwise.
 */
int	handle_direct_path(t_tocken *c_tocken)
{
	struct stat	st;

	if (ft_strchr(c_tocken->cmd_tb[0], '/') != 0)
	{
		if (stat(c_tocken->cmd_tb[0], &st) != -1 && S_ISDIR(st.st_mode))
			c_tocken->error_cmd = 3;
		else if (access(c_tocken->cmd_tb[0], F_OK | X_OK) == -1)
			c_tocken->error_cmd = 1;
		return (1);
	}
	return (0);
}

/**
 * @brief Searches for a command in the `PATH` environment variable.
 * 
 * This function retrieves the `PATH` variable from the environment, splits it
 * into individual directories, and searches for the command in each directory.
 * If the command is found, it returns the full path to the executable.
 * 
 * - Retrieves the `PATH` variable using `get_env_value`.
 * - Splits the `PATH` variable into directories using `ft_split`.
 * - Searches for the command in each directory using `find_exe_file`.
 * - Frees the allocated memory for the directory table.
 * 
 * @param x_file The command to search for.
 * @param env The environment table.
 * @return The full path to the executable, or `NULL` if not found.
 */
char	*check_path(char *x_file, char **env)
{
	char	**absolute_paths_table;
	char	*absolute_paths;
	char	*path;

	if (!x_file || x_file[0] == '\0' || !env)
		return (NULL);
	absolute_paths = get_env_value("PATH", env);
	if (!absolute_paths)
		return (NULL);
	absolute_paths_table = ft_split(absolute_paths, ':');
	if (!absolute_paths_table || !absolute_paths[0])
		return (NULL);
	path = find_exe_file(absolute_paths_table, x_file);
	if (!path)
		return (ft_free_table(absolute_paths_table), NULL);
	ft_free_table(absolute_paths_table);
	return (path);
}

/**
 * @brief Retrieves the value of an environment variable.
 * 
 * This function searches for a specific environment variable in the environment
 * table and returns its value. The variable is identified by its key (e.g., `PATH`).
 * 
 * - Constructs the key in the format `key=`.
 * - Iterates through the environment table to find a matching key.
 * 
 * @param key_value The key of the environment variable to search for.
 * @param env The environment table.
 * @return The value of the environment variable, or `NULL` if not found.
 */
char	*get_env_value(const char *key_value, char **env)
{
	int		i;
	size_t	key_len;
	char	*new_value;

	if (!key_value)
		return (NULL);
	key_len = ft_strlen(key_value);
	if (key_len == 0 || env == NULL)
		return (NULL);
	new_value = ft_strjoin(key_value, "=");
	if (new_value == NULL)
		return (NULL);
	i = 0;
	while (env[i] != NULL)
	{
		if ((ft_strncmp(new_value, env[i], key_len + 1)) == 0)
		{
			free(new_value);
			return ((env[i] + key_len + 1));
		}
		i++;
	}
	free(new_value);
	return (NULL);
}

/**
 * @brief Searches for an executable file in a list of directories.
 * 
 * This function iterates through a list of possible directories and checks
 * if the command exists and is accessible in any of them. If found, it returns
 * the full path to the executable.
 * 
 * - Combines each directory with the command name to form a full path.
 * - Checks if the file exists and is accessible using `access`.
 * - Frees intermediate memory allocations for each path.
 * 
 * @param posible_paths Array of directories to search in.
 * @param x_file The command to search for.
 * @return The full path to the executable, or `NULL` if not found.
 */
char	*find_exe_file(char **posible_paths, char *x_file)
{
	char	*relative_path;
	char	*aux;
	int		i;

	if (!posible_paths || !x_file)
		return (NULL);
	i = 0;
	while (posible_paths[i])
	{
		aux = ft_strjoin(posible_paths[i], "/");
		if (!aux)
			return (NULL);
		relative_path = ft_strjoin(aux, x_file);
		if (!relative_path)
		{
			free(aux);
			return (NULL);
		}
		if (access(relative_path, F_OK) != -1)
			return (free(aux), relative_path);
		i++;
		free(aux);
		free(relative_path);
	}
	return (NULL);
}
