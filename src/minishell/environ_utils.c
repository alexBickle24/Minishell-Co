/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 17:24:42 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/06 17:27:44 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//This functions is used in the process of convert the env in char **format
//to a list. alloc memery for the id.
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

//This funtion allocs memery for the value of a env_var once you hace the id
//we use it as well in the process to convert the env in char ** format to
//env in format list.
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

//This function searchs env vals by his id, if the node is in the env_list
//the function returns a pointer to the node, if not returns NULL
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

//Use: Create a new node of envinment list. You have to alloc memery for de 
//node and alloc memery for the id and the value.
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

//Once you create the node with the previus function, you can use this to add
//the node to de back of the list.
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
