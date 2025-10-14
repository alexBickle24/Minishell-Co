/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:51:44 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:06:06 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
