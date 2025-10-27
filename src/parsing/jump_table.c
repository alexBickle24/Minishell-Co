/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jump_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:12:33 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:53:09 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Handles single and double quotes during lexical analysis.
 * 
 * This function determines whether the current character is a single quote (`'`)
 * or a double quote (`"`), and calls the appropriate handler function:
 * - `s_quotes`: Handles single quotes.
 * - `d_quotes`: Handles double quotes.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 * @return The return value of the respective quote handler function.
 */
int	quotes(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	int	ret;

	if (line[*i] == '\'')
		ret = s_quotes(msl, i, line, pars);
	if (line[*i] == '\"')
		ret = d_quotes(msl, i, line, pars);
	return (ret);
}

/**
 * @brief Handles general information tokens (e.g., words or commands).
 * 
 * This function identifies and processes tokens that are not operators,
 * redirections, or quotes. It determines the start and end of a token
 * and creates a new lexical unit if necessary.
 * 
 * The function modifies the following states:
 * - `infstat`: Updated to `WORD` to indicate that the current token is a word.
 * - `parsstat`: The *syntactic label* is assigned and then the state is set
 *   to `T_CMD` to prepare for the next token.
 * 
 * When the start of a token is detected, it sets the `ptr` pointer to the
 * beginning of the token. When the end of a token is detected, it calls
 * `create_newlex`, which creates the token and assigns it a syntactic label.
 * The *syntactic label* is carried forward (arranged) until it is assigned.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 * @return Always returns 0.
 */
int	info(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	if (pars->lexstat == NO_QUOTES)
	{
		if (*i == 0 || (*i > 0 && (pars->sep_op[line[*i - 1]] > 0
					&& pars->sep_op[line[*i - 1]] < 5)))
		{
			pars->ptr = &line[*i];
		}
		if (line[*i + 1] == '\0' || (pars->sep_op[line[*i + 1]] > 0
				&& pars->sep_op[line[*i + 1]] < 5))
		{
			create_newlex(msl, i, line, pars);
			pars->parstat = T_CMD;
		}
	}
	pars->infstat = WORD;
	return ((*i)++, 0);
}

/**
 * @brief Handles the pipe operator (`|`) during lexical analysis.
 * 
 * This function processes the pipe operator and ensures that it is used
 * correctly in the input line. If the pipe operator is used incorrectly
 * (e.g., consecutive pipes), it returns an error. Otherwise, it creates
 * a new lexical unit for the pipe operator.
 * 
 * The function modifies the following states:
 * - `parsstat`: Updated to `T_PIPE` to indicate that the current token is a pipe.
 *   This is necessary for the parsing phase to determine when to create a new
 *   execution token.
 * - `infstat`: Updated to `OPERATOR` to reflect that the current token is an operator.
 * 
 * Lexicalizing the pipe operator is essential because it allows the parser
 * to identify execution boundaries and correctly group commands into separate
 * execution units (subshells) during the parsing phase.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 * @return 0 on success, or an error code if the pipe operator is used incorrectly.
 */
int	pipe_op(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	if (pars->lexstat == NO_QUOTES)
	{
		pars->parstat = T_PIPE;
		if (pars->infstat != WORD)
			return (get_unexpexted_errors1(pars->parstat, pars->infstat));
		else
		{
			pars->ptr = &line[*i];
			create_newlex(msl, i, line, pars);
		}
		pars->infstat = OPERATOR;
		pars->parstat = T_CMD;
	}
	else
		pars->infstat = WORD;
	return ((*i)++, 0);
}

/**
 * @brief Handles output redirection (`>` and `>>`) during lexical analysis.
 * 
 * This function processes output redirection operators and determines
 * whether the redirection is for overwriting (`>`) or appending (`>>`).
 * It also ensures that consecutive redirections are handled correctly.
 * 
 * The behavior of the function depends on whether the operator is inside quotes:
 * - If the operator is **not inside quotes** (`lexstat == NO_QUOTES`):
 *   - The `parsstat` state is updated to `T_OUTFILE` for `>` or `T_APPEND` for `>>`.
 *   - The `infstat` state is updated to `REDIR` to indicate that a redirection
 *     operator is being processed.
 *   - If consecutive redirections are detected, an error is returned.
 * - If the operator is **inside quotes**, it is treated as part of a word
 *   (e.g., `">"` is not a redirection but a string literal). In this case,
 *   the `infstat` state is set to `WORD`.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 * @return 0 on success, or an error code if consecutive redirections are detected.
 */
int	redir_out(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	if (pars->lexstat == NO_QUOTES)
	{
		pars->parstat = T_OUTFILE;
		if (line[*i + 1] == '>')
		{
			pars->parstat = T_APPEND;
			(*i)++;
		}
		if (pars->infstat == REDIR)
			return (get_unexpexted_errors1(pars->parstat, pars->infstat));
		pars->infstat = REDIR;
	}
	else
		pars->infstat = WORD;
	return ((void)msl, (*i)++, 0);
}

/**
 * @brief Handles input redirection (`<` and `<<`) during lexical analysis.
 * 
 * This function processes input redirection operators and determines
 * whether the redirection is for a file (`<`) or a heredoc (`<<`).
 * It also ensures that consecutive redirections are handled correctly.
 * 
 * The behavior of the function depends on whether the operator is inside quotes:
 * - If the operator is **not inside quotes** (`lexstat == NO_QUOTES`):
 *   - The `parsstat` state is updated to `T_INFILE` for `<` or to `T_HEREDOC`
 *     for `<<`.
 *   - The `infstat` state is updated to `REDIR` to indicate that a redirection
 *     operator is being processed.
 *   - If the operator is `<<`, the function calls `redir_in_extracases` to handle
 *     specific cases for heredocs and herestrings.
 *   - If consecutive redirections are detected, an error is returned.
 * - If the operator is **inside quotes**, it is treated as part of a word
 *   (e.g., `"<"` is not a redirection but a string literal). In this case,
 *   the `infstat` state is set to `WORD`.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param i Pointer to the current index in the input line.
 * @param line Pointer to the input line being analyzed.
 * @param pars Pointer to the parsing utilities structure.
 * @return 0 on success, or an error code if consecutive redirections are detected.
 */
int	redir_in(t_msl *msl, int *i, unsigned char *line, t_parsing *pars)
{
	if (pars->lexstat == NO_QUOTES)
	{
		pars->parstat = T_INFILE;
		if (line[*i + 1] == '<')
		{
			(*i)++;
			redir_in_extracases(i, line, pars);
		}
		if (pars->infstat == REDIR)
			return (get_unexpexted_errors1(pars->parstat, pars->infstat));
		pars->infstat = REDIR;
	}
	else
		pars->infstat = WORD;
	return ((void)msl, (*i)++, 0);
}
