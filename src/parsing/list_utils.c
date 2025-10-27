/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:39:46 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:53:55 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Creates a new token node for the executor structure.
 * 
 * This function allocates memory for a new `t_tocken` node and initializes
 * its fields to default values. The token represents a command group or
 * execution unit in the shell.
 * 
 * - `index`: Set to the provided position.
 * - `error_file` and `error_cmd`: Initialized to 0.
 * - `files`, `redir_in`, `redir_out`, `pcmds`, `cmd_tb`, and `env_tb`: Initialized to `NULL`.
 * - `next`: Set to `NULL` to indicate the end of the list.
 * 
 * @param position The position or index of the token in the execution sequence.
 * @return A pointer to the newly created token node, or `NULL` if memory allocation fails.
 */
t_tocken	*list_new_tocken(int position)
{
	t_tocken	*new_node;

	new_node = (t_tocken *)malloc(sizeof(t_tocken));
	if (!new_node)
		return (NULL);
	new_node->index = position;
	new_node->error_file = 0;
	new_node->files = NULL;
	new_node->redir_in = NULL;
	new_node->redir_out = NULL;
	new_node->pcmds = NULL;
	new_node->cmd_tb = NULL;
	new_node->env_tb = NULL;
	new_node->error_cmd = 0;
	new_node->next = NULL;
	return (new_node);
}

/**
 * @brief Creates a new file node for redirection handling.
 * 
 * This function allocates memory for a new `t_files` node and initializes
 * its fields to the provided values. The file node represents a file used
 * for input or output redirection in a command.
 * 
 * - `file_name`: Set to the provided file name.
 * - `type`: Set to the provided redirection type (e.g., input, output, append).
 * - `ambiguos`: Indicates whether the file name is ambiguous (1 = ambiguous, 0 = not ambiguous).
 * - `next`: Set to `NULL` to indicate the end of the list.
 * 
 * @param file The name of the file.
 * @param type The type of redirection (e.g., input, output, append).
 * @param ambiguos Flag indicating if the file name is ambiguous.
 * @return A pointer to the newly created file node, or `NULL` if memory allocation fails.
 */
t_files	*list_new_files(char *file, int type, char ambiguos)
{
	t_files	*new_node;

	new_node = (t_files *)malloc(sizeof(t_files));
	if (!new_node)
		return (NULL);
	new_node->file_name = file;
	new_node->type = type;
	new_node->ambiguos = ambiguos;
	new_node->next = NULL;
	return (new_node);
}

/**
 * @brief Creates a new command node for the command list.
 * 
 * This function allocates memory for a new `t_pcmds` node and initializes
 * its fields to the provided values. The command node represents a single
 * command or argument in the execution sequence.
 * 
 * - `cmd`: Set to the provided command string.
 * - `next`: Set to `NULL` to indicate the end of the list.
 * 
 * @param cmd The command or argument string.
 * @return A pointer to the newly created command node, or `NULL` if memory allocation fails.
 */
t_pcmds	*list_new_pcmds(char *cmd)
{
	t_pcmds	*new_node;

	new_node = (t_pcmds *)malloc(sizeof(t_pcmds));
	if (!new_node)
		return (NULL);
	new_node->cmd = cmd;
	new_node->next = NULL;
	return (new_node);
}

/**
 * @brief Adds a new file node to the end of the file list.
 * 
 * This function appends a new `t_files` node to the end of the file list.
 * If the list is empty, the new node becomes the head of the list.
 * 
 * - If the list is empty (`*list == NULL`), the new node is assigned as the head.
 * - Otherwise, the function iterates to the end of the list and appends the new node.
 * 
 * @param list Pointer to the pointer of the file list.
 * @param new_node Pointer to the new file node to be added.
 */
void	list_addback_infiles(t_files **list, t_files *new_node)
{
	t_files	*current;

	if (!new_node)
		return ;
	current = *list;
	if (!*list)
		*list = new_node;
	else
	{
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

/**
 * @brief Adds a new command node to the end of the command list.
 * 
 * This function appends a new `t_pcmds` node to the end of the command list.
 * If the list is empty, the new node becomes the head of the list.
 * 
 * - If the list is empty (`*list == NULL`), the new node is assigned as the head.
 * - Otherwise, the function iterates to the end of the list and appends the new node.
 * 
 * @param list Pointer to the pointer of the command list.
 * @param new_node Pointer to the new command node to be added.
 */
void	list_addback_pcmds(t_pcmds **list, t_pcmds *new_node)
{
	t_pcmds	*current;

	if (!new_node)
		return ;
	current = *list;
	if (!*list)
		*list = new_node;
	else
	{
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}
