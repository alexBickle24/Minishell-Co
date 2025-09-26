/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:04:03 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/23 21:05:40 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	concatenate_strings(char **str, int *i, size_t *len, char *replace)
{
	size_t	replace_len;
	char	*tmp;
	char	*tmp_after;
	char	*new;

	replace_len = ft_strlen(replace);
	(*str)[*i] = '\0';
	tmp = ft_strjoin(*str, replace);
	tmp_after = &((*str)[*i + 2]);
	new = ft_strjoin(tmp, tmp_after);
	free(replace);
	free(tmp);
	free(*str);
	*str = new;
	*len = *len + replace_len;
	*i = *i + replace_len;
}

void	concatenate_strings2(char **str, int *i, size_t *len, char *replace)
{
	size_t	replace_len;
	char	*tmp;
	char	*new;

	replace_len = ft_strlen(replace);
	tmp = ft_strjoin(*str, replace);
	new = ft_strjoin(tmp, &(*str)[(*i)]);
	*i = ft_strlen(new) - ft_strlen(&(*str)[(*i)]);
	free(tmp);
	free(*str);
	*str = new;
	*len = *len + replace_len;
}

void	home_case(char **str, t_msl *msl)
{
	t_env	*env_node;
	char	*home;

	env_node = search_id_node(msl, "HOME");
	if (!env_node)
		home = ft_strdup(msl->sys->home);
	else
		home = ft_strdup(env_node->value);
	if (!str || !*str)
		return ;
	if (str[0][0] == '~' && str[0][1] == '\0')
	{
		free(*str);
		*str = home;
	}
	else
		free(home);
}