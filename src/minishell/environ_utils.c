/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 17:24:42 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 15:08:54 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Extracts the `id` (key) from an environment variable string.
 * 
 * This function processes a string in the format `key=value` and extracts
 * the `key` portion. It allocates memory for the `id` and copies the
 * characters up to the `=` delimiter.
 * 
 * This function is used during the conversion of the environment from
 * a `char **` format to a linked list format.
 * 
 * @param env_line The environment variable string in `key=value` format.
 * @return A pointer to the newly allocated `id` string, or `NULL` on failure.
 */
char	*ft_get_env_id(char *env_line)
{
	char	*id;
	int		i;
	int		j;

	i = 0;
	while (env_line[i] && env_line[i] != '=')
		i++;
	id = (char *)malloc(sizeof(char) * (i + 1));
	if (!id)
		return (NULL);
	j = 0;
	while (j < i)
	{
		id[j] = env_line[j];
		j++;
	}
	id[j] = '\0';
	return (id);
}

/**
 * @brief Extracts the `value` from an environment variable string.
 * 
 * This function searches for a specific `id` (key) in the environment array
 * and extracts its corresponding `value`. It allocates memory for the `value`
 * and copies the characters after the `=` delimiter.
 * 
 * This function is used during the conversion of the environment from
 * a `char **` format to a linked list format.
 * 
 * @param env The environment array in `char **` format.
 * @param id The `id` (key) to search for in the environment array.
 * @return A pointer to the newly allocated `value` string, or `NULL` on failure.
 */
char	*ft_get_env_value(char **env, char *id)
{
	int		i;
	int		j;
	int		k;
	char	*value;

	if (!env || !id)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], id, ft_strlen_c(id)) == 0
			&& env[i][ft_strlen_c(id)] == '=')
		{
			j = ft_strlen_c(id) + 1;
			value = malloc(sizeof(char) * (ft_strlen(env[i]) - j + 1));
			if (!value)
				return (NULL);
			k = 0;
			while (env[i][j])
				value[k++] = env[i][j++];
			value[k] = '\0';
			return (value);
		}
	}
	return (NULL);
}

/**
 * @brief Searches for an environment variable node by its `id`.
 * 
 * This function iterates through the environment linked list to find a node
 * with a matching `id`. If the node is found, it returns a pointer to the node.
 * If no matching node is found, it returns `NULL`.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param id The `id` (key) to search for in the environment list.
 * @return A pointer to the matching `t_env` node, or `NULL` if not found.
 */
t_env	*search_id_node(t_msl *msl, char *id)
{
	t_env	*current;

	if (!msl)
		return (NULL);
	current = msl->own_env;
	while (current)
	{
		if (ft_strncmp(current->id, id, ft_strlen(id) + 1) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Creates a new node for the environment linked list.
 * 
 * This function allocates memory for a new `t_env` node and initializes its
 * fields with the provided `id` and `value`. The `next` pointer is set to `NULL`.
 * 
 * @param id The `id` (key) of the environment variable.
 * @param value The `value` of the environment variable.
 * @return A pointer to the newly created `t_env` node, or `NULL` on failure.
 */
t_env	*list_new_ownenv(char *id, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	node->id = id;
	node->value = value;
	node->next = NULL;
	return (node);
}

/**
 * @brief Adds a new node to the end of the environment linked list.
 * 
 * This function appends a new `t_env` node to the end of the environment
 * linked list. If the list is empty, the new node becomes the head of the list.
 * 
 * @param node Pointer to the new `t_env` node to be added.
 * @param env Pointer to the pointer of the environment linked list.
 */
void	list_addback_env(t_env *node, t_env **env)
{
	t_env	*current;

	if (!env)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = node;
	}
}
