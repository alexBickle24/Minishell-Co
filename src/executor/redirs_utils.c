/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:08:16 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:22:08 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Checks and sets up input redirections for a command.
 * 
 * This function iterates through the list of files associated with a token
 * and sets up input redirections (`redir_in`) for the command. It handles
 * heredocs, input files, and here-strings. If an error occurs (e.g., file
 * not found, permission denied, or ambiguous redirection), it stops processing.
 * 
 * - For input redirections (`T_HEREDOC`, `T_INFILE`, `T_HEREDOC_S`, `T_HERE_STR`):
 *   - Sets `redir_in` to the current file.
 *   - Checks file access permissions using `access`.
 * - Calls `check_out_redirs` to handle output redirections.
 * 
 * @param c_tocken Pointer to the current token being processed.
 * @param files_list Pointer to the list of files associated with the token.
 */
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

/**
 * @brief Checks and sets up output redirections for a command.
 * 
 * This function handles output redirections (`redir_out`) for a command.
 * It creates or appends to the specified output file, depending on the
 * redirection type (`T_OUTFILE` or `T_APPEND`). If an error occurs (e.g.,
 * ambiguous redirection or file creation failure), it stops processing.
 * 
 * - For output redirections (`T_OUTFILE`, `T_APPEND`):
 *   - Sets `redir_out` to the current file.
 *   - Opens the file with the appropriate flags:
 *     - `O_TRUNC` for overwriting (`T_OUTFILE`).
 *     - `O_APPEND` for appending (`T_APPEND`).
 *   - Closes the file descriptor after opening.
 * 
 * @param c_tocken Pointer to the current token being processed.
 * @param cur_f Pointer to the current file being checked.
 * @return `1` if an error occurs, `0` otherwise.
 */
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

/**
 * @brief Forwards input redirection for a command.
 * 
 * This function sets up the input redirection for a command based on the
 * `redir_in` field of the token. If no input redirection is specified, it
 * forwards the input from the previous pipe. If an error occurs (e.g.,
 * ambiguous redirection or file access failure), it sets the `error_file`
 * flag in the token.
 * 
 * - If `redir_in` is `NULL`:
 *   - Forwards input from the previous pipe using `pipe_forward`.
 * - If `redir_in` is set:
 *   - Closes the previous pipe.
 *   - Checks for ambiguous redirection or file access errors using `tunel_in_file`.
 *   - Calls `ft_error_redirs` to display an error message if necessary.
 * 
 * @param c_tocken Pointer to the current token being processed.
 */
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

/**
 * @brief Forwards output redirection for a command.
 * 
 * This function sets up the output redirection for a command based on the
 * `redir_out` field of the token. If no output redirection is specified, it
 * forwards the output to the next pipe. If an error occurs (e.g., ambiguous
 * redirection or file creation failure), it sets the `error_file` flag in the token.
 * 
 * - If `redir_out` is `NULL`:
 *   - Forwards output to the next pipe using `pipe_forward`.
 * - If `redir_out` is set:
 *   - Determines the redirection type (`T_APPEND` or `T_OUTFILE`).
 *   - Closes the next pipe.
 *   - Checks for ambiguous redirection or file creation errors using `tunel_out_file`.
 *   - Calls `ft_error_redirs` to display an error message if necessary.
 * 
 * @param c_tocken Pointer to the current token being processed.
 */
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
