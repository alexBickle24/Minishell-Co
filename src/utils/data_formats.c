/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_formats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 18:47:10 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:51:49 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Converts an environment linked list to a string table.
 * 
 * This function iterates through an environment linked list (`t_env`) and
 * creates a table of strings where each entry is formatted as `key=value`.
 * 
 * - Allocates memory for the table using `create_table1`.
 * - For each node in the environment list:
 *   - Joins the `id` and `value` fields with an `=` separator.
 *   - Adds the resulting string to the table.
 * - Frees temporary memory used during string concatenation.
 * - Ensures the table is null-terminated.
 * 
 * This function is used during the execution phase to prepare the environment
 * variables in the format required by `execve`.
 * 
 * @param env Pointer to the head of the environment linked list.
 * @return A pointer to the newly created string table, or `NULL` on failure.
 */
char	**ft_env_to_table(t_env *env)
{
	t_env	*current;
	int		lines;
	char	*tmp;
	char	**table;

	table = create_table1(env);
	if (table == NULL)
		return (NULL);
	current = env;
	lines = 0;
	while (current)
	{
		tmp = ft_strjoin(current->id, "=");
		if (!tmp)
			return (ft_free_table(table), NULL);
		table[lines] = ft_strjoin(tmp, current->value);
		if (!table[lines])
			return (NULL);
		free(tmp);
		current = current->next;
		lines++;
	}
	table[lines] = NULL;
	return (table);
}

/**
 * @brief Converts a command linked list to a string table.
 * 
 * This function iterates through a command linked list (`t_pcmds`) and
 * creates a table of strings where each entry corresponds to a command or
 * argument in the list.
 * 
 * - Allocates memory for the table using `create_table`.
 * - For each node in the command list:
 *   - Duplicates the `cmd` field and adds it to the table.
 * - Ensures the table is null-terminated.
 * - Frees the table and returns `NULL` if memory allocation fails.
 * 
 * This function is used during the execution phase to prepare the arguments
 * in the format required by `execve`.
 * 
 * @param pcmds Pointer to the head of the command linked list.
 * @return A pointer to the newly created string table, or `NULL` on failure.
 */
char	**ft_pcmds_to_table(t_pcmds *pcmds)
{
	t_pcmds	*current;
	int		lines;
	char	**table;

	table = create_table(pcmds);
	if (!table)
		return (NULL);
	current = pcmds;
	lines = 0;
	while (current)
	{
		table[lines] = ft_strdup(current->cmd);
		if (!table[lines])
			return (ft_free_table(table), NULL);
		current = current->next;
		lines++;
	}
	table[lines] = NULL;
	return (table);
}

/**
 * @brief Allocates memory for a string table based on the size of a command list.
 * 
 * This function calculates the number of nodes in a command linked list
 * (`t_pcmds`) and allocates memory for a string table with enough space
 * to store all commands plus a null terminator.
 * 
 * @param list Pointer to the head of the command linked list.
 * @return A pointer to the newly allocated string table, or `NULL` on failure.
 */
char	**create_table(t_pcmds *list)
{
	t_pcmds	*current;
	int		lines;
	char	**table;

	if (list == NULL)
		return (NULL);
	current = list;
	lines = 0;
	while (current)
	{
		current = current->next;
		lines++;
	}
	table = (char **)malloc(sizeof(char *) * (lines + 1));
	if (!table)
		return (NULL);
	return (table);
}

/**
 * @brief Allocates memory for a string table based on the size of an environment list.
 * 
 * This function calculates the number of nodes in an environment linked list
 * (`t_env`) and allocates memory for a string table with enough space to store
 * all `key=value` pairs plus a null terminator.
 * 
 * @param list Pointer to the head of the environment linked list.
 * @return A pointer to the newly allocated string table, or `NULL` on failure.
 */
char	**create_table1(t_env *list)
{
	t_env	*current;
	int		lines;
	char	**table;

	if (list == NULL)
		return (NULL);
	current = list;
	lines = 0;
	while (current)
	{
		current = current->next;
		lines++;
	}
	table = (char **)malloc(sizeof(char *) * (lines + 1));
	if (!table)
		return (NULL);
	return (table);
}
