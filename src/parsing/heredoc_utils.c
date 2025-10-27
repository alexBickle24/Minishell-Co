/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:51:44 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:52:28 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Generates a unique temporary file name.
 * 
 * This function creates a unique file name in the specified path by appending
 * an incrementing number to the path. It checks if the file already exists
 * using `access` and continues incrementing the number until a unique name
 * is found.
 * 
 * This mechanism is crucial for controlling concurrency when using a temporary
 * file as an IPC (Inter-Process Communication) mechanism. If multiple instances
 * of `minishell` are running with heredocs, each process must have its own
 * unique temporary file. Without this, the file offset in each process would
 * be reset, leading to data corruption and inconsistent information.
 * 
 * @param path The directory path where the file will be created.
 * @return A string containing the unique file name, or `NULL` if memory
 *         allocation fails.
 */
char	*new_file_name(char *path)
{
	int		name;
	char	*file_name;
	char	*name_str;

	name = 0;
	while (1)
	{
		file_name = ft_itoa(name);
		name_str = ft_strjoin(path, file_name);
		if (!name_str)
			return (free(file_name), NULL);
		if (access(name_str, F_OK))
			break ;
		name++;
		free(file_name);
		free(name_str);
	}
	free(file_name);
	return (name_str);
}

/**
 * @brief Sets the modes for heredoc processing.
 * 
 * This function determines the modes for heredoc processing based on the
 * delimiter and the `sangria` flag:
 * - `modes[0]`: Determines if variables are expanded (0 = expand, 1 = no expansion).
 *   This is set to `1` if the delimiter contains quotes, indicating a literal heredoc.
 * - `modes[1]`: Determines if leading tabs are stripped (1 = strip, 0 = keep).
 *   This is set to `1` if the `sangria` flag is enabled.
 * 
 * @param modes Array to store the heredoc modes.
 * @param delimiter The string used to terminate the heredoc input.
 * @param sangria Indicates whether leading tabs should be stripped.
 */
void	set_heredocs_modes(char *modes, char *delimiter, char sangria)
{
	if (have_quotes(delimiter))
		modes[0] = 1;
	else
		modes[0] = 0;
	if (sangria == 1)
		modes[1] = 1;
	else
		modes[1] = 0;
}

/**
 * @brief Displays a warning for an unexpected EOF in a heredoc.
 * 
 * This function prints a warning message to the standard error output when
 * a heredoc is terminated by an unexpected EOF instead of the expected
 * delimiter. The warning includes the line number and the expected delimiter.
 * 
 * @param delimiter The string used to terminate the heredoc input.
 */
void	ft_hwarningexit(char *delimiter)
{
	char	*n_line_history;

	n_line_history = ft_itoa(history_length);
	ft_putstr_fd("minishell: warning: here document at line ", 2);
	ft_putstr_fd(n_line_history, 2);
	free(n_line_history);
	ft_putstr_fd(" delimited by end-of-file (wanted '", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

/**
 * @brief Expands an environment variable and writes its value to a file.
 * 
 * This function processes a `$`-prefixed variable in a heredoc line, retrieves
 * its value from the environment, and writes the value to the specified file.
 * 
 * - The function uses the `dollar_lim` dictionary to determine the valid
 *   characters for the variable name.
 * - If the variable exists, its value is written to the file.
 * - If the variable does not exist, the `$` is treated as a literal character.
 * 
 * @param line The line containing the variable to be expanded.
 * @param fd File descriptor of the temporary file.
 * @param count The current index in the line.
 * @param msl Pointer to the main structure of the program.
 * @return The updated index after processing the variable.
 */
unsigned int	write_env(char *line, int fd, unsigned int count, t_msl *msl)
{
	char			*env_value;
	char			*env_name;
	char			**env;
	unsigned char	*linei;
	int				i;

	env = ft_env_to_table(msl->own_env);
	linei = (unsigned char *)line;
	i = 1;
	while (msl->parsing_utils->dollar_lim[linei[i]] >= 8)
		i++;
	count = count + i;
	env_name = ft_substr((const char *)line, 1, i - 1);
	if (!env_name)
		return (ft_free_table(env), 0);
	env_value = get_env_value(env_name, env);
	if (!env_value)
		return (free(env_name), ft_free_table(env), count - 1);
	ft_putstr_fd(env_value, fd);
	return (free(env_name), ft_free_table(env), count -1);
}

/**
 * @brief Creates a temporary file for a herestring and writes its content.
 * 
 * This function generates a temporary file in `/tmp/` to store the content
 * of a herestring. The content is written to the file, followed by a newline.
 * 
 * - If the input line is `NULL` or the global signal `g_signal` is not in
 *   the `S_INIT` state, the function returns `NULL`.
 * - The file is created with read and write permissions, and the content is
 *   written using `ft_putstr_fd`.
 * 
 * @param line The content of the herestring to be written.
 * @return The name of the temporary file created, or `NULL` on failure.
 */
char	*create_here_str(char *line)
{
	int		fd;
	char	*file_name;

	if (line == NULL || g_signal != S_INIT)
		return (NULL);
	file_name = new_file_name("/tmp/");
	if (file_name == NULL)
		return (NULL);
	fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0664);
	if (fd < 0)
		return (NULL);
	ft_putstr_fd(line, fd);
	ft_putchar_fd('\n', fd);
	close (fd);
	return (file_name);
}
