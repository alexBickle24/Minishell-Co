/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:53:04 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/16 15:30:39 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_heredoc(t_msl *msl, char *delimiter, char sangr)
{
	char	modes[2];
	int		fd;
	char	*file_name;
	char	*tmp;

	if (g_signal == S_SIGINT)
		return (NULL);
	file_name = new_file_name("/tmp/");
	fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0664);
	if (fd < 0)
		return (NULL);
	set_heredocs_modes(modes, delimiter, sangr);
	tmp = delimiter;
	while (*delimiter)
	{
		if (check_clean_quotes(msl, delimiter, 1))
			delimiter++;
	}
	delimiter = tmp;
	heredoc_child_process(msl, fd, delimiter, modes);
	return (close(fd), file_name);
}

void	heredoc_child_process(t_msl *msl, int fd, char *delimiter, char *modes)
{
	pid_t	pid;

	g_signal = S_HEREDOC;
	pid = fork();
	if (pid < 0)
	{
		ft_errerrno();
		return ;
	}
	if (pid == 0)
		heredoc_loop(msl, delimiter, fd, modes);
	if (pid > 0)
		wait_childs4(msl, pid);
}

void	heredoc_loop(t_msl *msl, char *delimiter, int fd, char *modes)
{
	char	*line;

	signal_init_heredoc();
	while (1)
	{
		line = readline("> ");
		if (!line)
			ft_hwarningexit(delimiter);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write_line_in_heredoc(line, fd, msl, modes);
		free(line);
	}
	close (fd);
	exit (0);
}

void	write_line_in_heredoc(char *line, int fd, t_msl *msl, char *modes)
{
	unsigned int	i;
	char			*t_line;

	if (!line)
		return ;
	if (modes[1] == 1)
		t_line = jump_caracter(line, '\t');
	else
		t_line = line;
	i = -1;
	while (t_line[++i] != '\0')
	{
		if (t_line[i] == '$' && modes[0] == 0)
			i = write_dollar_cases(t_line, msl, fd, i);
		else
		{
			ft_putchar_fd(t_line[i], fd);
		}
	}
	write(fd, "\n", 1);
}

int	write_dollar_cases(char *t_line, t_msl *msl, int fd, int i)
{
	char			*dolim;
	unsigned char	idx;

	dolim = msl->parsing_utils->dollar_lim;
	idx = (unsigned char)t_line[i + 1];
	if (dolim[idx] > 0 && dolim[idx] <= 6)
		ft_putchar_fd(t_line[i], fd);
	else if (dolim[idx] <= 8)
	{
		if (t_line[i + 1] == '$')
			ft_putnbr_fd(msl->msl_pid, fd);
		else if (t_line[i + 1] == '!')
			ft_putnbr_fd(msl->last_process, fd);
		else if (t_line[i + 1] == '?')
			ft_putnbr_fd(msl->exit_status, fd);
		else if (t_line[i + 1] == '0')
			ft_putstr_fd(INTERPRETER_NAME, fd);
		i++;
	}
	else if (dolim[idx] == 9)
		i++;
	else if (dolim[idx] == 10)
		i = write_env(&t_line[i], fd, i, msl);
	return (i);
}
