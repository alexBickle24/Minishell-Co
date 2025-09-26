/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:42:37 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/25 01:13:57 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	vars_interpolation(char **str, t_msl *msl, size_t *len)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (1)
	{
		check_clean_quotes(msl, &((*str)[i]), 0);
		if ((*str)[i] == '$' && msl->parsing_utils->lexstat != S_QUOTES)
			dollar_expansion(str, &i, len, msl);
		else
			i++;
		if ((*str)[i] == '\0')
			break ;
	}
}

void	dollar_expansion(char **str, int *i, size_t *len, t_msl *msl)
{
	char			*dolim;
	unsigned char	*idx;

	idx = (unsigned char *)(*str);
	dolim = msl->parsing_utils->dollar_lim;
	if (dolim[idx[*i + 1]] >= 1 && dolim[idx[*i + 1]] < 5)
		(*i)++;
	else if (dolim[idx[*i + 1]] == 5)
	{
		if (msl->parsing_utils->lexstat == NO_QUOTES)
		{
			ft_memmove(&((*str)[*i]), &((*str)[*i + 1]), *len - *i);
			(*len)--;
		}
		else
			(*i)++;
	}
	else if (dolim[idx[*i + 1]] == 6 || (dolim[idx[*i + 1]] == 0))
		(*i)++;
	else if (dolim[idx[*i + 1]] >= 7)
		dollar_expansion2(str, i, len, msl);
}

void	dollar_expansion2(char **str, int *i, size_t *len, t_msl *msl)
{
	char			*dolim;
	unsigned char	*idx;

	idx = (unsigned char *)(*str);
	dolim = msl->parsing_utils->dollar_lim;
	if (dolim[idx[*i + 1]] >= 7 && dolim[idx[*i + 1]] <= 8)
		replace_dollar(str, i, len, msl);
	else if (dolim[idx[*i + 1]] == 9)
	{
		ft_memmove(&((*str)[*i]), &((*str)[*i + 2]), *len - *i);
		*i = *i + 2;
		*len -= 2;
	}
	else if (dolim[idx[*i + 1]] == 10)
		expand_vars(str, i, len, msl);
}

void	replace_dollar(char **str, int *i, size_t *len, t_msl *msl)
{
	char	*replace;

	replace = NULL;
	if ((*str)[*i + 1] == '$')
		replace = ft_itoa(msl->msl_pid);
	else if ((*str)[*i + 1] == '!')
		replace = ft_itoa(msl->last_process);
	else if ((*str)[*i + 1] == '?')
		replace = ft_itoa(msl->exit_status);
	else if ((*str)[*i + 1] == '0')
		replace = ft_strdup(INTERPRETER_NAME);
	else
		*len = *len - 2;
	concatenate_strings(str, i, len, replace);
}

void	expand_vars(char **str, int *i, size_t *len, t_msl *msl)
{
	t_env	*env_node;
	char	tmp;
	size_t	i_tmp;
	char	*new;

	i_tmp = ++(*i);
	while (msl->parsing_utils->dollar_lim[(unsigned char)(*str)[i_tmp]] >= 8)
		i_tmp++;
	tmp = (*str)[i_tmp];
	(*str)[i_tmp] = '\0';
	env_node = search_id_node(msl, &((*str)[*i]));
	(*str)[i_tmp] = tmp;
	(*str)[--(*i)] = '\0';
	*len = *len - (i_tmp - *i);
	if (env_node == NULL)
	{
		new = ft_strjoin(*str, &((*str)[i_tmp]));
		free(*str);
		*str = new;
	}
	else
	{
		*i = i_tmp;
		concatenate_strings2(str, i, len, env_node->value);
	}
}
