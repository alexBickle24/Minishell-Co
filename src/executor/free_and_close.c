/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:42:24 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:45:42 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Closes both ends of a pipe.
 * 
 * This function closes the read (`pipe_ports[0]`) and write (`pipe_ports[1]`)
 * ends of a pipe if the `pipe_ports` array is not `NULL`.
 * 
 * @param pipe_ports Pointer to an array containing the file descriptors of the pipe.
 */
void	close_fds(int *pipe_ports)
{
	if (pipe_ports)
	{
		close(pipe_ports[0]);
		close(pipe_ports[1]);
	}
}

/**
 * @brief Frees a null-terminated array of strings.
 * 
 * This function iterates through a null-terminated array of strings, frees
 * each string, and then frees the array itself. It ensures that all pointers
 * are set to `NULL` after being freed to avoid dangling pointers.
 * 
 * @param ptr Pointer to the null-terminated array of strings to be freed.
 */
void	ft_free_table(char **ptr)
{
	int	i;

	if (ptr == NULL)
		return ;
	i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
	ptr = NULL;
}

/**
 * @brief Frees the file list associated with a token.
 * 
 * This function iterates through the list of files (`t_files`) associated
 * with a token and frees the memory allocated for each file. If the file
 * is a heredoc, it also unlinks (deletes) the temporary file from the filesystem.
 * 
 * - Frees the `file_name` field of each file node.
 * - Unlinks temporary files for heredocs (`T_HEREDOC`, `T_HEREDOC_S`, `T_HERE_STR`).
 * - Frees each file node and sets the `files` pointer to `NULL`.
 * 
 * @param tocken Pointer to the token whose file list will be freed.
 */
void	free_tocken_files(t_tocken *tocken)
{
	t_files	*files;
	t_files	*tmp;

	files = tocken->files;
	if (!files)
		return ;
	while (files)
	{
		if ((files->type == T_HEREDOC || files->type == T_HEREDOC_S
				|| files->type == T_HERE_STR) && files->file_name)
			unlink(files->file_name);
		if (files->file_name)
			free(files->file_name);
		tmp = files;
		files = files->next;
		free(tmp);
	}
	tocken->files = NULL;
}

/**
 * @brief Frees the command list associated with a token.
 * 
 * This function iterates through the list of commands (`t_pcmds`) associated
 * with a token and frees the memory allocated for each command. It ensures
 * that all memory is properly released and sets the `pcmds` pointer to `NULL`.
 * 
 * - Frees the `cmd` field of each command node.
 * - Frees each command node and sets the `pcmds` pointer to `NULL`.
 * 
 * @param tocken Pointer to the token whose command list will be freed.
 */
void	free_tocken_cmds(t_tocken *tocken)
{
	t_pcmds	*pcmds;
	t_pcmds	*tmp;

	pcmds = tocken->pcmds;
	while (pcmds)
	{
		tmp = pcmds->next;
		if (pcmds->cmd)
			free(pcmds->cmd);
		free(pcmds);
		pcmds = tmp;
	}
	tocken->pcmds = NULL;
}

/**
 * @brief Frees the entire token list and its associated resources.
 * 
 * This function iterates through the list of tokens (`t_tocken`) in the shell
 * structure and frees all associated resources, including:
 * - Command tables (`cmd_tb`).
 * - Environment tables (`env_tb`).
 * - Command lists (`pcmds`).
 * - File lists (`files`).
 * 
 * It also resets the `redir_in` and `redir_out` pointers and sets the token
 * list pointer (`msl->tocken`) to `NULL`. The total token count is reset to `0`.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
void	free_tockens(t_msl *msl)
{
	t_tocken	*c_tocken;
	t_tocken	*tmp;

	if (!msl)
		return ;
	c_tocken = msl->tocken;
	while (c_tocken)
	{
		if (c_tocken->cmd_tb)
			ft_free_table(c_tocken->cmd_tb);
		if (c_tocken->env_tb)
			ft_free_table(c_tocken->env_tb);
		free_tocken_cmds(c_tocken);
		free_tocken_files(c_tocken);
		c_tocken->redir_in = NULL;
		c_tocken->redir_out = NULL;
		tmp = c_tocken;
		c_tocken = c_tocken->next;
		free(tmp);
	}
	msl->tocken = NULL;
	msl->total_tockens = 0;
}
