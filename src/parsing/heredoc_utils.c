#include "../../inc/minishell.h"

void ft_hwarningexit(char *delimiter)
{
	char *n_line_history;

	n_line_history = ft_itoa(history_length);
	ft_putstr_fd("minishell: warning: here document at line ", 2);
	ft_putstr_fd(n_line_history, 2);
	free(n_line_history);
	ft_putstr_fd(" delimited by end-of-file (wanted '", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')", 2);
}

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



unsigned int	write_env(char *line, int fd, unsigned int count, t_msl *msl)
{
	char		*env_value;
	char		*env_name;
	char		**env;
	unsigned char *linei;
	int			i;

	env = ft_env_to_table(msl->own_env);
	linei = (unsigned char *)line;
	i = 1;
	while (!(msl->parsing_utils->dollar_lim[linei[i]] < 8 && (msl->parsing_utils->dollar_lim[linei[i]] > 0)))
		i++;
	count = count + i;
	env_name = ft_substr((const char *)line, 1, i - 1);
	if (!env_name)
		return (ft_free_table(env), 0);
	env_value = get_env_value(env_name, env);
	if (!env_value)
	{
		free(env_name);
		ft_free_table(env);
		return (count - 1);
	}
	ft_putstr_fd(env_value, fd);
	return (free(env_name), ft_free_table(env), count -1);
}
