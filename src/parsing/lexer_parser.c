/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:54:48 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:53:29 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Performs lexical and syntactical analysis on the input line.
 * 
 * This function processes the input line character by character, using
 * the lexical analysis jump table (`lex`) to handle each character based
 * on its type (as defined in the `sep_op` dictionary). It stops processing
 * if an error is encountered or when the end of the line is reached.
 * 
 * If an error occurs during lexical analysis, the lexer resources are freed.
 * Otherwise, the function manages the final parsing state and resets the
 * parsing utilities for the next operation.
 * 
 * @note The function performs lexicalization, parsing, and error checking in a
 * single pass over the string. By using a jump table associated with a
 * dictionary, it achieves constant time complexity (O(1)) for character
 * classification and processing.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param line Pointer to the input line to be analyzed.
 */
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

/**
 * @brief Handles the final state of the parser after lexical analysis.
 * 
 * This function checks the final state of the parser to determine if
 * there are any syntax errors. It handles the following cases:
 * - If the last state is a redirection (`REDIR`), it sets a parsing error flag.
 * - If the last state is an operator (`OPERATOR`), it prints an error message
 *   and frees the lexer resources.
 * - If there are unmatched quotes (`NO_QUOTES`), it prints an error message
 *   and frees the lexer resources.
 * 
 * @note The error flag for redirection is used to mimic Bash's behavior:
 * - Bash creates heredocs even when there is a redirection error, then deletes
 *   them and displays the error message.
 * - However, when there are consecutive operators in the line, Bash does not
 *   expand heredocs. This behavior is replicated here to ensure compatibility
 *   with Bash's lexicalization and parsing phases.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param parser Pointer to the parsing utilities structure.
 */
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
