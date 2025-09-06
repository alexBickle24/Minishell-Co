/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_home_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:02:13 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/05 22:07:51 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	close (fd);
}

char	*find_home(char *line)
{
	char	**user_data;
	char	*home;
	int		i;

	user_data = ft_split(line, ':');
	if (!user_data)
		return (free(line), NULL);
	i = 0;
	while (user_data[i] != NULL)
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
		i++;
	}
	ft_free_table(user_data);
	return (home);
}

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
