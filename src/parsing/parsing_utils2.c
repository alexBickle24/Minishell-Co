/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:55:50 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:54:23 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Checks if a string contains quotes.
 * 
 * This function iterates through a string to determine if it contains
 * single quotes (`'`) or double quotes (`"`).
 * 
 * @param str Pointer to the string to be checked.
 * @return 1 if the string contains quotes, 0 otherwise.
 */
int	have_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Removes all quotes from a string.
 * 
 * This function iterates through a string and removes all single quotes (`'`)
 * and double quotes (`"`). It uses `ft_memmove` to shift the remaining characters
 * and adjust the string length.
 * 
 * @param str Pointer to the string to be cleaned.
 * @return Pointer to the modified string with quotes removed, or `NULL` if the input is `NULL`.
 */
char	*clean_quotes(char *str)
{
	char	*tmp;
	int		lensrc;
	int		i;

	tmp = str;
	if (!tmp)
		return (NULL);
	lensrc = (int)ft_strlen(str);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\'' || tmp[i] == '\"')
		{
			ft_memmove(&tmp[i], &tmp[i + 1], lensrc - i);
			lensrc--;
		}
		i++;
	}
	return (str);
}

/**
 * @brief Skips over consecutive occurrences of a specific character in a string.
 * 
 * This function advances the pointer in the string while the current character
 * matches the specified `caracter`.
 * 
 * @param str Pointer to the string to be processed.
 * @param caracter The character to skip over.
 * @return Pointer to the first character in the string that does not match `caracter`.
 */
char	*jump_caracter(char *str, char caracter)
{
	while (str && *str && *str == caracter)
		str++;
	return (str);
}

/**
 * @brief Skips over whitespace characters in a string.
 * 
 * This function advances the pointer in the string while the current character
 * is a whitespace character (e.g., space, tab, newline).
 * 
 * @param str Pointer to the pointer of the string to be processed.
 */
void	jump_separator(char **str)
{
	while (str && *str && is_space(**str))
		(*str)++;
}

/**
 * @brief Checks if a character is a whitespace character.
 * 
 * This function determines if the given character is a whitespace character,
 * including spaces, tabs, and other control characters (`\t`, `\n`, `\v`, `\f`, `\r`).
 * 
 * @param c The character to be checked.
 * @return 1 if the character is a whitespace character, 0 otherwise.
 */
int	is_space(char c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	return (0);
}
