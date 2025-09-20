/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jump_table2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:54:38 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/20 17:18:59 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	s_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	pars->infstat = WORD;
	if (pars->lexstat == NO_QUOTES)
	{
		pars->lexstat = S_QUOTES;
		if (*i == 0 || (*i > 0 && (pars->sep_op[line[*i - 1]] > 0
					&& pars->sep_op[line[*i - 1]] < 5)))
		{
			pars->ptr = &line[*i];
		}
	}
	else if (pars->lexstat == S_QUOTES)
	{
		pars->lexstat = NO_QUOTES;
		if (line[*i + 1] == '\0' || (pars->sep_op[line[*i + 1]] > 0
				&& pars->sep_op[line[*i + 1]] < 5))
		{
			create_newlex(msl, i, line, pars);
			pars->parstat = T_CMD;
		}
	}
	return ((*i)++, 0);
}

int	d_quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	pars->infstat = WORD;
	if (pars->lexstat == NO_QUOTES)
	{
		pars->lexstat = D_QUOTES;
		if (*i == 0 || (*i > 0 && (pars->sep_op[line[*i - 1]] > 0
					&& pars->sep_op[line[*i - 1]] < 5)))
		{
			pars->ptr = &line[*i];
		}
	}
	else if (pars->lexstat == D_QUOTES)
	{
		pars->lexstat = NO_QUOTES;
		if (line[*i + 1] == '\0' || (pars->sep_op[line[*i + 1]] > 0
				&& pars->sep_op[line[*i + 1]] < 5))
		{
			create_newlex(msl, i, line, pars);
			pars->parstat = T_CMD;
		}
	}
	return ((*i)++, 0);
}

int	spaces(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	(void)msl;
	(void)line;
	(void)pars;
	return ((*i)++, 0);
}

void	create_newlex(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	char	tmp;
	t_lex	*new_node;

	if (msl->mode == 1)
	{
		if (pars->parstat == T_HEREDOC || pars->parstat == T_HEREDOC_S)
		{
			free_msl(&msl);
			ft_hwarningexit("EOF");
			exit (0);
		}
	}
	tmp = line[*i + 1];
	line[*i + 1] = '\0';
	new_node = lex_newnode(pars->parstat, ft_strdup((const char *)pars->ptr));
	addback_lex(msl, new_node);
	line[*i + 1] = tmp;
}

void	redir_in_extracases(int *i, unsigned char *line, t_parsing *pars)
{
	if (line[*i + 1] == '-')
	{
		pars->parstat = T_HEREDOC_S;
		(*i)++;
		if (pars->sep_op[line[*i + 1]] == 0 || pars->sep_op[line[*i + 1]] == 5)
			pars->ptr = &line[*i + 1];
	}
	else if (line[*i + 1] == '<')
	{
		pars->parstat = T_HERE_STR;
		(*i)++;
	}
	else
		pars->parstat = T_HEREDOC;
}
