/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:53:04 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:52:41 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Creates a temporary file for a heredoc and writes its content.
 * 
 * This function generates a temporary file in `/tmp/` to store the heredoc
 * content. It sets the appropriate modes for heredoc processing, handles
 * quoted delimiters, and spawns a child process to read and write the heredoc
 * content.
 * 
 * - If a `SIGINT` signal is detected, the function returns `NULL`.
 * - The delimiter is processed to handle quotes using `check_clean_quotes`.
 * - The actual heredoc content is written in the child process via
 *   `heredoc_child_process`.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param delimiter The string used to terminate the heredoc input.
 * @param sangria Indicates whether leading tabs should be stripped.
 * @return The name of the temporary file created, or `NULL` on failure.
 */
char	*create_heredoc(t_msl *msl, char *delimiter, char sangria)
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
	set_heredocs_modes(modes, delimiter, sangria);
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

/**
 * @brief Handles the heredoc content in a child process.
 * 
 * This function forks a child process to handle the heredoc input. The child
 * process reads lines from the user until the delimiter is encountered and
 * writes the content to the temporary file. The parent process waits for the
 * child to finish.
 * 
 * - The `g_signal` is set to `S_HEREDOC` to indicate heredoc processing.
 * - If the fork fails, an error is printed using `ft_errerrno`.
 * - The child process executes `heredoc_loop` to handle the heredoc input.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param fd File descriptor of the temporary file.
 * @param delimiter The string used to terminate the heredoc input.
 * @param modes Array indicating heredoc modes (e.g., expansion, stripping tabs).
 */
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

/**
 * @brief Reads and processes heredoc input in a loop.
 * 
 * This function continuously reads lines from the user until the delimiter
 * is encountered. Each line is processed and written to the temporary file.
 * If the user sends an EOF signal, a warning is displayed, and the process exits.
 * 
 * - The `signal_init_heredoc` function is called to set up heredoc-specific signals.
 * - If the delimiter is matched, the loop breaks, and the file is closed.
 * - Each line is processed by `write_line_in_heredoc` to handle variable expansion
 *   and other modes.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param delimiter The string used to terminate the heredoc input.
 * @param fd File descriptor of the temporary file.
 * @param modes Array indicating heredoc modes (e.g., expansion, stripping tabs).
 */
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

/**
 * @brief Writes a processed line to the heredoc file.
 * 
 * This function processes a single line of heredoc input and writes it to
 * the temporary file. It handles variable expansion and tab stripping based
 * on the heredoc modes.
 * 
 * - If `modes[1] == 1`, leading tabs are stripped using `jump_caracter`.
 * - If `modes[0] == 0`, variables of the form `$<variable>` are expanded.
 *   This behavior depends on whether the heredoc is in literal mode or not.
 *   Literal heredocs (e.g., those with quoted delimiters) do not expand variables.
 * - All other characters are written directly to the file.
 * 
 * The function iterates through the line character by character:
 * - If a `$` is encountered and `modes[0] == 0`, the function calls
 *   `write_dollar_cases` to handle variable expansion.
 * - Otherwise, the character is written directly to the file.
 * 
 * @param line The line to be written.
 * @param fd File descriptor of the temporary file.
 * @param msl Pointer to the main structure of the program.
 * @param modes Array indicating heredoc modes:
 *   - `modes[0]`: Determines if variables are expanded (0 = expand, 1 = no expansion).
 *   - `modes[1]`: Determines if leading tabs are stripped (1 = strip, 0 = keep).
 */
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

/**
 * @brief Handles variable expansion in heredoc lines.
 * 
 * This function processes `$` in heredoc lines and expands variables based
 * on the `dollar_limits` dictionary. It handles the following cases:
 * - `$?`: Expands to the exit status of the last command.
 * - `$$`: Expands to the process ID of the shell.
 * - `$!`: Expands to the process ID of the last background command.
 * - `$0`: Expands to the name of the interpreter.
 * - `$<variable>`: Expands to the value of the environment variable.
 * - `$<invalid>`: Writes `$` directly if the character following `$` is not valid.
 * 
 * The function uses the `dollar_limits` dictionary to determine the behavior
 * for each character following `$`. Special cases like `<<` and `<<<` are
 * handled separately.
 * 
 * @param t_line The current heredoc line being processed.
 * @param msl Pointer to the main structure of the program.
 * @param fd File descriptor of the temporary file.
 * @param i Current index in the line.
 * @return The updated index after processing the `$` case.
 */
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
