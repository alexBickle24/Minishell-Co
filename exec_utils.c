/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:46:29 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:04:52 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
