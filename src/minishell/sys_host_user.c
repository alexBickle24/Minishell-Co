/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_host_user.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:50:04 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/05 22:01:00 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
