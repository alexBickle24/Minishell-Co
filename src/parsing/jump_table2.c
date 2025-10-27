/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jump_table2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:54:38 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:53:23 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Handles single quotes during lexical analysis.
 * 
 * This function processes single quotes (`'`) in the input line. It toggles
 * the `lexstat` state between `NO_QUOTES` and `S_QUOTES` to track whether
 * the parser is inside or outside single quotes. Single quotes prevent
 * variable expansion and treat everything inside as a literal string.
 * 
 * - When entering single quotes (`NO_QUOTES -> S_QUOTES`):
 *   - Sets `pars->ptr` to the start of the token if the previous character
 *     is a separator or the beginning of the line.
 * - When exiting single quotes (`S_QUOTES -> NO_QUOTES`):
 *   - Calls `create_newlex` to create a new token and assigns it a syntactic
 *     label.
 *   - Updates `parsstat` to `T_CMD` to prepare for the next token.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 * @return Always returns 0.
 */
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

/**
 * @brief Handles double quotes during lexical analysis.
 * 
 * This function processes double quotes (`"`) in the input line. It toggles
 * the `lexstat` state between `NO_QUOTES` and `D_QUOTES` to track whether
 * the parser is inside or outside double quotes. Double quotes allow variable
 * expansion but treat everything else as a literal string.
 * 
 * - When entering double quotes (`NO_QUOTES -> D_QUOTES`):
 *   - Sets `pars->ptr` to the start of the token if the previous character
 *     is a separator or the beginning of the line.
 * - When exiting double quotes (`D_QUOTES -> NO_QUOTES`):
 *   - Calls `create_newlex` to create a new token and assigns it a syntactic
 *     label.
 *   - Updates `parsstat` to `T_CMD` to prepare for the next token.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 * @return Always returns 0.
 */
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

/**
 * @brief Handles spaces and other whitespace characters during lexical analysis.
 * 
 * This function skips over spaces and other whitespace characters in the input
 * line. It does not modify any parsing states or create tokens, as spaces are
 * treated as delimiters between tokens.
 * 
 * @param msl Pointer to the main structure of the program (unused).
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed (unused).
 * @param pars Pointer to the parsing utilities structure (unused).
 * @return Always returns 0.
 */
int	spaces(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	(void)msl;
	(void)line;
	(void)pars;
	return ((*i)++, 0);
}

/**
 * @brief Creates a new lexical token and adds it to the token list.
 * 
 * This function creates a new token based on the current parsing state and
 * the substring defined by `pars->ptr` and the current index. The token is
 * assigned a syntactic label (`pars->parstat`) and added to the token list.
 * 
 * - Temporarily modifies the input line to extract the token string.
 * - Calls `lex_newnode` to create a new token node.
 * - Calls `addback_lex` to append the new token to the token list.
 * - Restores the input line after extracting the token.
 * 
 * If the shell is in heredoc mode (`T_HEREDOC` or `T_HEREDOC_S`) and the
 * `msl->mode` indicates that we are in interpreter mode (mode 1), heredocs
 * are not processed. Instead, the function frees resources, displays an
 * EOF warning, and exits the program.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 */
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

/**
 * @brief Handles special cases for input redirection (`<<`).
 * 
 * This function processes specific cases for heredocs and herestrings:
 * - `<<-`: Treated as a stripped heredoc (`T_HEREDOC_S`), where leading tabs
 *   are ignored in the heredoc content. The pointer to the start of the lexer
 *   (`pars->ptr`) is adjusted to the end of the string because this is a
 *   special case.
 * - `<<<`: Treated as a herestring (`T_HERE_STR`), where the content is
 *   directly expanded.
 * - `<<`: Default heredoc (`T_HEREDOC`).
 * 
 * The function updates `pars->parstat` to the appropriate state and adjusts
 * the index to skip over the extra characters.
 * 
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 */
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
