/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_home_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:02:13 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:48:50 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Retrieves the home directory for the current user.
 * 
 * This function searches for the home directory of the current user in the
 * `/etc/passwd` file. If the home directory is found, it is assigned to the
 * `target` pointer. If not, it falls back to a hardcoded default using
 * `hardcoding_home`.
 * 
 * - Opens `/etc/passwd` for reading.
 * - Iterates through each line of the file using `get_next_line`.
 * - Compares the username in the file with the current user (`msl->sys->user`).
 * - If a match is found, calls `find_home` to extract the home directory.
 * - If no home directory is found, calls `hardcoding_home` as a fallback.
 * - Closes the file descriptor after processing.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param target Pointer to the string where the home directory will be stored.
 */
void	get_home(t_msl *msl, char **target)
{
	int		fd;
	char	*line;

	fd = open("/etc/passwd", O_RDONLY);
	if (fd < 0)
		return ;
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (!ft_strncmp(line, msl->sys->user, ft_strlen(msl->sys->user)))
		{
			*target = find_home(line);
			if (*target == NULL)
				break ;
		}
		free (line);
	}
	if (*target == NULL)
		hardcoding_home(msl, target);
	close (fd);
}

/**
 * @brief Extracts the home directory from a line in `/etc/passwd`.
 * 
 * This function splits a line from `/etc/passwd` into fields using `:` as
 * the delimiter. It searches for a field that starts with `/home` and
 * duplicates it as the home directory.
 * 
 * - Splits the line into fields using `ft_split`.
 * - Iterates through the fields to find one starting with `/home`.
 * - If found, duplicates the field using `ft_strdup` and returns it.
 * - Frees the line and the split fields before returning.
 * 
 * @param line A line from `/etc/passwd` containing user information.
 * @return A pointer to the duplicated home directory string, or `NULL` on failure.
 */
char	*find_home(char *line)
{
	char	**user_data;
	char	*home;
	int		i;

	user_data = ft_split(line, ':');
	if (!user_data)
		return (free(line), NULL);
	i = -1;
	home = NULL;
	while (user_data[++i] != NULL)
	{
		if (!ft_strncmp(user_data[i], "/home", ft_strlen("/home")))
		{
			home = ft_strdup(user_data[i]);
			if (!home)
			{
				free(line);
				ft_free_table(user_data);
				return (NULL);
			}
			break ;
		}
	}
	ft_free_table(user_data);
	return (home);
}

/**
 * @brief Retrieves the global `PATH` variable from `/etc/environment`.
 * 
 * This function searches for the `PATH` variable in the `/etc/environment`
 * file and assigns its value to the `target` pointer.
 * 
 * - Opens `/etc/environment` for reading.
 * - Iterates through each line of the file using `put_global_path`.
 * - Closes the file descriptor after processing.
 * 
 * @param target Pointer to the string where the global `PATH` will be stored.
 */
void	get_global_path(char **target)
{
	int	fd;

	fd = open("/etc/environment", O_RDONLY);
	if (fd < 0)
		return ;
	while (1)
	{
		if (put_global_path(target, fd) != 0)
			break ;
	}
	close (fd);
}

/**
 * @brief Extracts the `PATH` variable from a line in `/etc/environment`.
 * 
 * This function checks if a line starts with `PATH=` and, if so, extracts
 * the value of the `PATH` variable. The value is duplicated and stored in
 * the `target` pointer.
 * 
 * - Reads a line from the file using `get_next_line`.
 * - Checks if the line starts with `PATH`.
 * - If a match is found, extracts the value after `PATH=` and duplicates it.
 * - Removes the trailing newline character from the duplicated string.
 * - Frees the line and returns `1` to indicate success.
 * - If no match is found, frees the line and returns `0`.
 * 
 * @param target Pointer to the string where the `PATH` value will be stored.
 * @param fd File descriptor of the `/etc/environment` file.
 * @return `1` if the `PATH` variable is found, `0` otherwise.
 */
char	put_global_path(char **target, int fd)
{
	char	*tmp;
	char	*line;
	size_t	len;

	line = get_next_line(fd);
	if (line == NULL)
		return (1);
	if (!ft_strncmp(line, "PATH", 4))
	{
		tmp = line + 5;
		*target = ft_strdup(tmp);
		if (!*target)
		{
			free(line);
			return (1);
		}
		len = ft_strlen(*target);
		if (len > 0)
			(*target)[len - 1] = '\0';
		free(line);
		return (1);
	}
	free(line);
	return (0);
}

/**
 * @brief Sets a hardcoded default home directory.
 * 
 * This function constructs a default home directory path using the format
 * `/home/<user>` and assigns it to the `target` pointer.
 * 
 * - Concatenates `/home/` with the username (`msl->sys->user`) using `ft_strjoin`.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param target Pointer to the string where the hardcoded home directory will be stored.
 */
void	hardcoding_home(t_msl	*msl, char	**target)
{
	*target = ft_strjoin("/home/", msl->sys->user);
	if (*target)
		return ;
}
