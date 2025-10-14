/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:55:34 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:06:56 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	check_nwl_error(t_msl *msl)
{
	t_lex	*lexer;

	if (msl->pars_err == 1)
	{
		lexer = msl->lexer;
		while (lexer)
		{
			if (lexer->type == T_HEREDOC || lexer->type == T_HEREDOC_S)
				create_heredoc_nwlerr(msl, lexer);
			lexer = lexer->next;
		}
		new_line_err(msl);
		msl->pars_err = 0;
		return (1);
	}
	return (0);
}

char	new_line_err(t_msl *msl)
{
	char	error;

	error = 0;
	if (msl->pars_err == 1)
	{
		error = 1;
		ft_putstr_fd(NEWLINE_ERR, 2);
		free_lexer(msl, 1);
		free_tockens(msl);
	}
	return (error);
}

void	create_heredoc_nwlerr(t_msl *msl, t_lex *lex)
{
	free(lex->str);
	if (lex->type == T_HEREDOC)
		lex->str = create_heredoc(msl, lex->raw, 0);
	else if (lex->type == T_HEREDOC_S)
		lex->str = create_heredoc(msl, lex->raw, 1);
}

void	list_addback_tocken(t_tocken **list, t_tocken *new_node)
{
	t_tocken	*current;

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
