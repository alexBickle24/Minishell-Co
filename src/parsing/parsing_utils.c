/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:55:47 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:54:13 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Handles the state transitions for quotes and optionally removes them.
 * 
 * This function checks the type of quote (`'` or `"`) at the current position
 * in the string and updates the lexer state (`lexstat`) accordingly. It also
 * calls the appropriate helper function (`check_clean_squotes` or
 * `check_clean_dquotes`) to handle the specific quote type.
 * 
 * - Single quotes (`'`) prevent variable expansion and are handled by `check_clean_squotes`.
 * - Double quotes (`"`) allow variable expansion and are handled by `check_clean_dquotes`.
 * - If the `clean` flag is set, the function removes the quote from the string.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param str Pointer to the current position in the string being processed.
 * @param clean Flag indicating whether the quote should be removed from the string.
 * @return 1 if no state transition occurs, or the result of the helper function.
 */
char	check_clean_quotes(t_msl *msl, char *str, char clean)
{
	char	jump;

	jump = 1;
	if (*str == '\'')
		jump = check_clean_squotes(msl, str, clean);
	if (*str == '\"')
		jump = check_clean_dquotes(msl, str, clean);
	return (jump);
}

/**
 * @brief Handles single quotes and optionally removes them.
 * 
 * This function updates the lexer state (`lexstat`) when encountering single
 * quotes (`'`). Single quotes prevent variable expansion and treat everything
 * inside as a literal string. The function toggles the state between `NO_QUOTES`
 * and `S_QUOTES`.
 * 
 * - If `lexstat == NO_QUOTES`, the state changes to `S_QUOTES` (entering single quotes).
 * - If `lexstat == S_QUOTES`, the state changes to `NO_QUOTES` (exiting single quotes).
 * - If the `clean` flag is set, the single quote is removed from the string.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param str Pointer to the current position in the string being processed.
 * @param clean Flag indicating whether the single quote should be removed from the string.
 * @return 0 if the state changes, 1 otherwise.
 */
char	check_clean_squotes(t_msl *msl, char *str, char clean)
{
	t_parsing	*pars;

	pars = msl->parsing_utils;
	if (pars->lexstat == NO_QUOTES)
	{
		pars->lexstat = S_QUOTES;
		if (clean)
			ft_memmove(str, str + 1, ft_strlen(str));
		return (0);
	}
	else if (pars->lexstat == S_QUOTES)
	{
		pars->lexstat = NO_QUOTES;
		if (clean)
			ft_memmove(str, str + 1, ft_strlen(str));
		return (0);
	}
	return (1);
}

/**
 * @brief Handles double quotes and optionally removes them.
 * 
 * This function updates the lexer state (`lexstat`) when encountering double
 * quotes (`"`). Double quotes allow variable expansion but treat everything
 * else inside as a literal string. The function toggles the state between
 * `NO_QUOTES` and `D_QUOTES`.
 * 
 * - If `lexstat == NO_QUOTES`, the state changes to `D_QUOTES` (entering double quotes).
 * - If `lexstat == D_QUOTES`, the state changes to `NO_QUOTES` (exiting double quotes).
 * - If the `clean` flag is set, the double quote is removed from the string.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param str Pointer to the current position in the string being processed.
 * @param clean Flag indicating whether the double quote should be removed from the string.
 * @return 0 if the state changes, 1 otherwise.
 */
char	check_clean_dquotes(t_msl *msl, char *str, char clean)
{
	t_parsing	*pars;

	pars = msl->parsing_utils;
	if (pars->lexstat == NO_QUOTES)
	{
		pars->lexstat = D_QUOTES;
		if (clean)
			ft_memmove(str, str + 1, ft_strlen(str));
		return (0);
	}
	else if (pars->lexstat == D_QUOTES)
	{
		pars->lexstat = NO_QUOTES;
		if (clean)
			ft_memmove(str, str + 1, ft_strlen(str));
		return (0);
	}
	return (1);
}
