/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:08:16 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/26 22:52:32 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	check_create_redirs(t_tocken *c_tocken, t_files *files_list)
{
	t_files	*current_f;

	current_f = files_list;
	if (!files_list)
		return ;
	while (current_f)
	{
		if (current_f->type == T_HEREDOC || current_f->type == T_INFILE
			|| current_f->type == T_HEREDOC_S || current_f->type == T_HERE_STR)
		{
			c_tocken->redir_in = current_f;
			if (access(current_f->file_name, F_OK | R_OK) == -1
				|| current_f->ambiguos)
				break ;
		}
		if (check_out_redirs(c_tocken, current_f))
			break ;
		current_f = current_f->next;
	}
}

int	check_out_redirs(t_tocken *c_tocken, t_files *cur_f)
{
	int	fd;

	if (cur_f->type == T_OUTFILE || cur_f->type == T_APPEND)
	{
		c_tocken->redir_out = cur_f;
		if (cur_f->ambiguos)
			return (1);
		if (cur_f->type == T_OUTFILE)
			fd = open(cur_f->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		else
			fd = open(cur_f->file_name, O_WRONLY | O_CREAT | O_APPEND, 0664);
		close(fd);
		if (fd == -1 || cur_f->ambiguos)
			return (1);
	}
	return (0);
}

void	fordward_in(t_tocken *c_tocken)
{
	t_files	*red_in;

	red_in = c_tocken->redir_in;
	if (!red_in)
	{
		if (c_tocken->index != 1)
		{
			if (pipe_forward(c_tocken->pipe_fds, 0, STDIN_FILENO))
				ft_errerrno();
		}
	}
	else
	{
		if (c_tocken->index != 1)
			close_fds(c_tocken->pipe_fds);
		if (red_in->ambiguos || tunel_in_file(red_in->file_name))
		{
			ft_error_redirs(red_in->file_name, red_in->ambiguos);
			c_tocken->error_file = 1;
		}
	}
}

void	fordward_out(t_tocken *c_tocken)
{
	char	ap_m;
	t_files	*red_out;

	ap_m = 0;
	red_out = c_tocken->redir_out;
	if (!red_out)
	{
		if (c_tocken->next != NULL)
		{
			if (pipe_forward(c_tocken->next->pipe_fds, 1, STDOUT_FILENO))
				ft_errerrno();
		}
	}
	else
	{
		if (red_out->type == T_APPEND)
			ap_m = 1;
		if (c_tocken->next != NULL)
			close_fds(c_tocken->next->pipe_fds);
		if (red_out->ambiguos || tunel_out_file(red_out->file_name, ap_m) == 1)
		{
			ft_error_redirs(red_out->file_name, red_out->ambiguos);
			c_tocken->error_file = 1;
		}
	}
}
