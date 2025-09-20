/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:54:56 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/20 17:23:51 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	get_unexpexted_errors1(int type, int infstat)
{
	if ((infstat == INIT || infstat == REDIR) && type == T_PIPE)
		ft_error_unexpect("|");
	else if (infstat == OPERATOR || infstat == REDIR)
	{
		if (type == T_HEREDOC)
			ft_error_unexpect("<<");
		if (type == T_HEREDOC_S)
			ft_error_unexpect("<<-");
		if (type == T_INFILE)
			ft_error_unexpect("<");
		if (type == T_OUTFILE)
			ft_error_unexpect(">");
		if (type == T_APPEND)
			ft_error_unexpect(">>");
		if (type == T_PIPE)
			ft_error_unexpect("|");
	}
	return (1);
}

void	ft_error_unexpect(char *simbol)
{
	ft_putstr_fd("minishell: syntax error near unexpected token", 2);
	ft_putstr_fd(" `", 2);
	ft_putstr_fd(simbol, 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd("\n", 2);
}

t_lex	*lex_newnode(int type, char *raw)
{
	t_lex	*node;

	node = (t_lex *)malloc(sizeof(t_lex));
	if (node == NULL)
		return (NULL);
	node->raw = raw;
	node->len = ft_strlen(raw);
	node->type = type;
	node->next = NULL;
	node->str = ft_strdup(raw);
	return (node);
}

void	addback_lex(t_msl *msl, t_lex *node)
{
	t_lex	*current;

	current = msl->lexer;
	if (current == NULL)
	{
		msl->lexer = node;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = node;
}

void	free_lexer(t_msl *msl, char all)
{
	t_lex	*lexer;
	t_lex	*tmp;

	lexer = msl->lexer;
	while (lexer)
	{
		if ((lexer->type == T_HEREDOC || lexer->type == T_HEREDOC_S
				|| lexer->type == T_HERE_STR) && lexer->str != NULL)
			unlink(lexer->str);
		if (all == 1)
		{
			if (lexer->raw != NULL)
				free (lexer->raw);
			if (lexer->str != NULL)
				free (lexer->str);
		}
		tmp = lexer->next;
		free(lexer);
		lexer = tmp;
	}
	msl->lexer = NULL;
	set_parsdefaultvals(msl);
}
