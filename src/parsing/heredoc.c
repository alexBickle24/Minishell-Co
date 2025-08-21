#include "../../inc/minishell.h"

char *create_heredoc(t_msl *msl, char *delimiter, char sangria)
{
	char modes[2];
	int fd;
	char *file_name;

	if (g_signal == SIGINT)
		return (NULL);
	file_name = new_file_name("/tmp/");
	fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0664);
	if (fd < 0)
		return (NULL);
	if(have_quotes(delimiter))
	{
		clean_quotes(delimiter);
		modes[0] = 1;
	}
	else
	modes[0] = 0;
	if (sangria == 1)
		modes[1] = 1;
	else
		modes[1] = 0;
	heredoc_child_process(msl, fd, delimiter, modes);
	close(fd);
	return(file_name);
}

void heredoc_child_process(t_msl *msl, int fd, char *delimiter, char *modes)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		ft_errerrno();
		return ;
	}
	if (pid == 0)
		heredoc_loop(msl, delimiter, fd, modes);
	wait_heredoc();
}

void heredoc_loop(t_msl *msl, char *delimiter, int fd, char *modes)
{
	char *line;

	signal_init_heredoc();
	while (1)
	{	
		line = readline("> ");
		if (!line)
			ft_hwarningexit(delimiter);
		add_history((const char *)line);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);;
			exit (0);
		}
		write_line_in_heredoc(line, fd, msl, modes);
		free (line);
	}
}

void	write_line_in_heredoc(char *line, int fd, t_msl *msl, char *modes)
{
	unsigned int	i;
	char			*t_line;//trimed line

	if (!line)
		return ;
	if (modes[1] == 1)//modo de sangria desactivado
		t_line = jump_caracter(line, '\t');
	else
		t_line = line;
	i = -1;
	while (t_line[++i] != '\0')
	{
		if (t_line[i] == '$' && modes[0] == 0)//que le modo literal este desactivado
			i = write_dollar_cases(t_line, msl, fd, i);
		else
		{
			ft_putchar_fd(t_line[i], fd);
		}
	}
	write(fd, "\n", 1);
}


//mod parseo, msl->env / fd->newbuffer / i -> &i return -> newstr
int	write_dollar_cases(char *t_line, t_msl *msl, int fd, int i)
{
	char *dolim;
	unsigned char idx;

	dolim = msl->parsing_utils->dollar_lim;
	idx = (unsigned char)t_line[i+1];
	if (dolim[idx] == 0)
		i = write_env(&t_line[i], fd, i, msl);
	else if (dolim[idx] > 0 && dolim[idx] <= 6)
		ft_putchar_fd(t_line[i], fd);
	else if (dolim[idx] <= 8)
	{
		if (t_line[i + 1] == '$')
			ft_putnbr_fd(msl->msl_pid, fd);//pid minishell
		else if (t_line[i + 1] == '?')
			ft_putnbr_fd(msl->last_process, fd);//pid del ultimo proceso (opcional)
		else if (t_line[i + 1] == '?')
			ft_putnbr_fd(msl->exit_status, fd);//exit_status
		else if (t_line[i + 1] == '0')
			ft_putstr_fd(INTERPRETER_NAME, fd);
		i++;
	}
	if (dolim[idx] == 9)
		i++;
	return (i);
}

//mod parseo, msl->env / fd->newbuffer / i -> &i return -> newstr
// int	write_dollar_cases(char *t_line, t_msl *msl, int fd, int i)
// {
// 	if (t_line[i + 1] == '$')
// 	{
// 		ft_putnbr_fd(msl->msl_pid, fd);//pid minishell
// 		i++;
// 	}
// 	else if (t_line[i + 1] == '?')
// 	{
// 		ft_putnbr_fd(msl->exit_status, fd);//exit_status
// 		i++;
// 	}
// 	else if (ft_isdigit(t_line[i + 1]))//si es un numero se lo salta
// 		i++;
// 	else if (ft_isalpha(t_line[i+1]))
// 		i = write_env(&t_line[i], fd, i, ft_env_to_table(msl->own_env));
// 	else
// 		ft_putchar_fd(t_line[i], fd);
// 	return (i);
// }