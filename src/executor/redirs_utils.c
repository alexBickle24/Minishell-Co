


#include "../../inc/minishell.h"


///Funcion para evaluar redirecciones: SE  LLAMA DESDE EL PADRE
//poner los tres casoso y porque en ninguno de los tres se dejan fds abiertos en le hijo
//y en el caso tres tendria que ser ina fucnion tipo int o char;
void check_create_redirs(t_tocken *c_tocken, t_files *files_list)
{
	t_files *current_f;
	int fd; 

	current_f = files_list;
	if (!files_list)
		return ;
	while (current_f)
	{
		if (current_f->type == HEREDOC || current_f->type == INFILE)
		{
			c_tocken->redir_in = current_f;
			if (access(current_f->file_name, F_OK | R_OK) == -1)
				break ;
		}
		if (current_f->type == OUTFILE || current_f->type == APPEND)
		{
			c_tocken->redir_out = current_f;
			if (current_f->type == OUTFILE)
				fd = open(current_f->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
			else
				fd = open(current_f->file_name, O_WRONLY | O_CREAT | O_APPEND, 0664);
			close(fd);
			if (fd == -1)
				break ;
		}
		current_f = current_f->next;
	}
}

int	tunel_in_file(char *file)
{
	int	fd_trgt;

	if (!file)
		return (2);
	if (access(file, F_OK | R_OK) == -1)
		return (1);
	fd_trgt = open(file, O_RDONLY, 0664);
	if (fd_trgt < 0)
		return (1);
	if (dup2(fd_trgt, STDIN_FILENO) == -1)
	{
		close(fd_trgt);
		return (1);
	}
	close(fd_trgt);
	return (0);
}

int	tunel_out_file(char *file, char append)
{
	int	fd_trgt;

	if (!file)
		return (2);
	if (append)
		fd_trgt = open(file, O_WRONLY | O_CREAT | O_APPEND, 0664);
	else
		fd_trgt = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_trgt < 0)
		return (1);
	if (dup2(fd_trgt, STDOUT_FILENO) == -1)
	{
		close(fd_trgt);
		return (1);
	}
	close(fd_trgt);
	return (0);
}

int	pipe_forward(int *pipe_reference, int pipe_port, int fd)
{
	int		other_fd;

	if (pipe_port == 0)
		other_fd = 1;
	else
		other_fd = 0;
	close(pipe_reference[other_fd]);
	if (dup2(pipe_reference[pipe_port], fd) == -1)
		return(close(pipe_reference[pipe_port]), 1);
	return(close(pipe_reference[pipe_port]), 0);
}

void fordward_in(t_tocken *c_tocken)
{
	if (!c_tocken->redir_in)
	{
		if(c_tocken->index != 1)//sino no redireccion y lee de la stdin
		{
			if (pipe_forward(c_tocken->pipe_fds, 0, STDIN_FILENO))//no configura errno si falla pero no tiene que interrumpir ejecion
				ft_errerrno();
		}
	}
	else
	{
		if(c_tocken->index != 1)
			close_fds(c_tocken->pipe_fds);
		if (tunel_in_file(c_tocken->redir_in->file_name))
		{
			ft_error_redirs(c_tocken->redir_in->file_name);
			c_tocken->error_file = 1;
		}
	}
}

void fordward_out(t_tocken *c_tocken)
{
	char append_mode;

	append_mode = 0;
	if (!c_tocken->redir_out)
	{
		if(c_tocken->next != NULL)
		{
			if (pipe_forward(c_tocken->next->pipe_fds, 1, STDOUT_FILENO))
				ft_errerrno();
		}
	}
	else
	{
		if (c_tocken->redir_out->type == APPEND)
			append_mode = 1;
		if(c_tocken->next != NULL)
			close_fds(c_tocken->next->pipe_fds);
		if (tunel_out_file(c_tocken->redir_out->file_name, append_mode) == 1)
		{
			ft_error_redirs(c_tocken->redir_out->file_name);
			c_tocken->error_file = 1;
		}
	}
}


//Con estas manerade hacerloimitamos bash como hace strace
// void check_create_redirs2(t_tocken *c_tocken, t_files *files_list)
// {
// 	t_files *current_f;
// 	int fd_in;
// 	int fd_out; 

// 	current_f = files_list;
// 	if (!files_list)
// 		return ;
// 	while (current_f)
// 	{
// 		if (current_f->type == HEREDOC || current_f->type == INFILE)
// 		{
// 			fd_in = open(current_f->file_name, O_RDONLY, 0664);
// 			if (fd_in == -1)
// 			{
// 				ft_error_redirs(current_f->file_name);
// 				exit(1);
// 			}
// 			else
// 			{
// 				if (dup2(fd_in, STDIN_FILENO) == -1)
// 				{
// 					close(fd_in);
// 					exit (1);
// 				}
// 				close(fd_in);
// 			}
// 		}
// 		if (current_f->type == OUTFILE || current_f->type == APPEND)
// 		{
// 			if (current_f->type == OUTFILE)
// 				fd_out = open(current_f->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
// 			else
// 				fd_out = open(current_f->file_name, O_WRONLY | O_CREAT | O_APPEND, 0664);
// 			if (fd_out == -1)
// 			{
// 				ft_error_redirs(current_f->file_name);
// 				exit(1);
// 			}
// 			else
// 			{
// 				if (dup2(fd_out, STDOUT_FILENO) == -1)
// 				{
// 					close(fd_out);
// 					exit (1);
// 				}
// 				close(fd_out);
// 			}
// 		}
// 		current_f = current_f->next;
// 	}
// }

// void fordward_pipes(t_tocken *c_tocken)
// {
// 	if(c_tocken->index != 1)//sino no redireccion y lee de la stdin
// 	{
// 		if (pipe_forward(c_tocken->pipe_fds, 0, STDIN_FILENO))//no configura errno si falla pero no tiene que interrumpir ejecion
// 			ft_errerrno();
// 	}
// 	if(c_tocken->next != NULL)
// 	{
// 		if (pipe_forward(c_tocken->next->pipe_fds, 1, STDOUT_FILENO))
// 			ft_errerrno();
// 	}
// }

