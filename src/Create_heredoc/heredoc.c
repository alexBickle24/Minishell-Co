

//This function creates heredoc and write on it
char *create_heredoc(char *string, char *h_delimiter, char **env)
{
    static char *l;
    char *src_name;
    int   fd_inter;

    src_name = new_file_name("/tmp/");
	fd_inter = open(src_name, O_CREAT | O_TRUNC | O_APPEND | O_RDWR, 0664);
	if (fd_inter < 0)
		return (free(src_name), NULL);
    //write in heredoc until EOF apprears
	write(1, ">", 1);
    //maybe we have to change get_next_line for readline function
	l = get_next_line(0);
	while (ft_strncmp(l, h_delimiter, ft_strlen(h_delimiter)) != 0
		|| (ft_strlen(l) - 1) != ft_strlen(h_delimiter))
	{
		write_line_in_heredoc(l, fd_inter, env);
		free(l);
		write(1, ">", 1);
		l = get_next_line(0);
	}
	return (free(l), close (fd_inter), src_name);
}

// This function handles all possible cases of heredoc parsing: expanding environment variables,
// writing the dollar symbol, or writing any other type of character.
// It does not execute commands because that is not required in Minishell.
// We need to check if any case has been missed.
void	write_line_in_heredoc(char *line, int fd_inter, char **env)
{
	unsigned int	i;

	if (!line)
		return ;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
		{
			if ((line[i + 1] != '(' && line[i + 1] != ' '
					&& line[i + 1] != '\n' && line[i + 1] != '$'))
			{
				i = write_env(&line[i], fd_inter, i, env);
			}
			else
				ft_putchar_fd(line[i], fd_inter);
		}
		else
			ft_putchar_fd(line[i], fd_inter);
		i++;
	}
}

char	*trim_line(char *line)
{
	int		i;

	i = 0;
	while (line[i] != ')')
		i++;
	line = ft_substr(line, 2, i - 2);
	if (!line)
		return (NULL);
	return (line);
}

// This function can be reused. It handles the case when a '$' character is found inside a heredoc.
// There are several possible behaviors, but I need to check if I missed any exceptions.
// On the other hand, parts of it could also be reused for variable expansion outside of the heredoc.
unsigned int	write_env(char *line, int fd, unsigned int count, char **env)
{
	char		*env_value;
	char		*env_name;
	int			i;

	i = 0;
	while (line[i] != ' ' && line[i] != '\n')
		i++;
	count = count + i;
	env_name = ft_substr((const char *)line, 1, i -1);
	if (!env_name)
		return (0);

	env_value = get_env_value(env_name, env);
	if (!env_value)
	{
		free(env_name);
		return (count - 1);
	}
	ft_putstr_fd(env_value, fd);
	return (free(env_name), count - 1);
}

//This function is for create a new file, been sure that the file dosent exist. We create the file in the tmp directory
//because is the way to bee sure that if the file isnt remove he system will do for us.
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
			return (NULL);
		if (access(name_str, F_OK))
			break ;
		name++;
		free(file_name);
		free(name_str);
	}
	free(file_name);
	return (name_str);
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

