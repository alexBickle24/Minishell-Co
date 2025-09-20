/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:54:48 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/20 17:21:10 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	lexer_parser(t_msl *msl, unsigned char *line)
{
	t_parsing	*parser;
	int			i;
	int			err;

	if (g_signal != S_INIT)
		return ;
	i = 0;
	err = 0;
	parser = msl->parsing_utils;
	while (line[i])
	{
		err = parser->lex[parser->sep_op[line[i]]](msl, &i, line, parser);
		if (err)
			break ;
	}
	if (err)
		free_lexer(msl, 1);
	else
		manage_last_state(msl, parser);
	set_parsdefaultvals(msl);
}

void	manage_last_state(t_msl *msl, t_parsing *parser)
{
	if (parser->infstat == REDIR)
		msl->pars_err = 1;
	else if (parser->infstat == OPERATOR)
	{
		ft_putstr_fd(MIPIPE_ERR, 2);
		free_lexer(msl, 1);
	}
	else if (parser->lexstat != NO_QUOTES)
	{
		ft_putstr_fd(MIQUOTE_ERR, 2);
		free_lexer(msl, 1);
	}
}
