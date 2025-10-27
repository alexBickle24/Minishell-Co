/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:43:16 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:54:47 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Splits a command string into individual arguments.
 * 
 * This function iterates through a command string, splitting it into
 * individual arguments based on spaces. It handles quotes to ensure that
 * arguments enclosed in single or double quotes are treated as a single unit.
 * 
 * - If a space is encountered outside of quotes, the current argument is
 *   terminated, added to the `pcmds` list of the current token, and the
 *   pointer advances to the next argument.
 * - Calls `check_clean_quotes` to handle quotes and determine if the current
 *   character is part of a quoted string.
 * - Skips over consecutive spaces using `jump_separator`.
 * 
 * This function is particularly useful when the argument is the result of
 * expanding a variable of environment that contains separators, ensuring
 * that the expanded value is split correctly into individual arguments.
 * 
 * @param str Pointer to the current position in the command string.
 * @param start Pointer to the start of the current argument.
 * @param msl Pointer to the main structure of the program.
 * @param cur Pointer to the current token being processed.
 */
void	loop_cmd_split(char **str, char **start, t_msl *msl, t_tocken *cur)
{
	char	jump;

	while (**str)
	{
		jump = check_clean_quotes(msl, *str, 1);
		if (is_space(**str) && msl->parsing_utils->lexstat == NO_QUOTES)
		{
			**str = '\0';
			list_addback_pcmds(&(cur->pcmds),
				list_new_pcmds(ft_strdup(*start)));
			(*str)++;
			jump_separator(str);
			*start = *str;
		}
		else if (jump == 1)
			(*str)++;
	}
}

/**
 * @brief Checks if a file name is ambiguous.
 * 
 * This function determines if a file name is ambiguous by checking for
 * spaces outside of quotes. If spaces are found, the file name is considered
 * ambiguous, as it cannot be interpreted as a single valid file name.
 * 
 * - Calls `check_clean_quotes` to handle quotes and ensure that spaces
 *   inside quotes are ignored.
 * - If a space is encountered outside of quotes, the function returns `1`
 *   to indicate an ambiguous file name.
 * 
 * This function is particularly useful when the file name is the result of
 * expanding a variable of environment that contains separators, ensuring
 * that the expanded value is validated correctly.
 * 
 * @param str Pointer to the file name string to be checked.
 * @param msl Pointer to the main structure of the program.
 * @return 1 if the file name is ambiguous, 0 otherwise.
 */
char	check_ambiguos_file(char *str, t_msl *msl)
{
	char	jump;

	if (*str == '\0')
		return (1);
	while (*str)
	{
		jump = check_clean_quotes(msl, str, 1);
		if (is_space(*str) && msl->parsing_utils->lexstat == NO_QUOTES)
			return (1);
		if (jump == 1)
			str++;
	}
	return (0);
}
