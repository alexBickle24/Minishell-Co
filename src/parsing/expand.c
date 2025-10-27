/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:42:37 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:52:16 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Expands variables and handles quotes in a string.
 * 
 * This function iterates through a string, expanding variables (e.g., `$VAR`)
 * and handling quotes. It checks for `$` characters and calls `dollar_expansion`
 * to process them. Single quotes prevent expansion, while double quotes allow it.
 * 
 * - If a `$` is found and the lexer state is not `S_QUOTES`, variable expansion is performed.
 * - Handles the end of the string to avoid out-of-bounds errors.
 * - The index is always moved to the end of the expanded variable to prevent
 *   infinite loops in cases where the expanded value contains another `$`.
 * 
 * @param str Pointer to the string to be processed.
 * @param msl Pointer to the main structure of the program.
 * @param len Pointer to the length of the string.
 */
void	vars_interpolation(char **str, t_msl *msl, size_t *len)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (1)
	{
		check_clean_quotes(msl, &((*str)[i]), 0);
		if ((*str)[i] == '$' && msl->parsing_utils->lexstat != S_QUOTES)
			dollar_expansion(str, &i, len, msl);
		else
			i++;
		if ((*str)[i] == '\0')
			break ;
	}
}

/**
 * @brief Handles the expansion of `$` in a string.
 * 
 * This function processes `$` characters in a string based on the `dollar_lim`
 * dictionary, which defines the behavior for characters following `$`:
 * - Codes 1-4: Skip the `$` character.
 * - Code 5: Remove the `$` if not in quotes.
 * - Codes 6 or 0: Skip the `$` without expansion.
 * - Codes >= 7: Calls `dollar_expansion2` for further processing.
 * 
 * The index is always moved to the end of the expanded variable to avoid
 * infinite loops in cases where the expanded value contains another `$`.
 * 
 * 
 * @param str Pointer to the string being processed.
 * @param i Pointer to the current index in the string.
 * @param len Pointer to the length of the string.
 * @param msl Pointer to the main structure of the program.
 */
void	dollar_expansion(char **str, int *i, size_t *len, t_msl *msl)
{
	char			*dolim;
	unsigned char	*idx;

	idx = (unsigned char *)(*str);
	dolim = msl->parsing_utils->dollar_lim;
	if (dolim[idx[*i + 1]] >= 1 && dolim[idx[*i + 1]] < 5)
		(*i)++;
	else if (dolim[idx[*i + 1]] == 5)
	{
		if (msl->parsing_utils->lexstat == NO_QUOTES)
		{
			ft_memmove(&((*str)[*i]), &((*str)[*i + 1]), *len - *i);
			(*len)--;
		}
		else
			(*i)++;
	}
	else if (dolim[idx[*i + 1]] == 6 || (dolim[idx[*i + 1]] == 0))
		(*i)++;
	else if (dolim[idx[*i + 1]] >= 7)
		dollar_expansion2(str, i, len, msl);
}

/**
 * @brief Handles advanced cases of `$` expansion.
 * 
 * This function processes `$` characters with codes >= 7 in the `dollar_lim`
 * dictionary:
 * - Codes 7-8: Calls `replace_dollar` to replace special variables (e.g., `$$`, `$?`).
 * - Code 9: Removes the `$` and the following character.
 * - Code 10: Calls `expand_vars` to expand environment variables.
 * 
 *  * The index is always moved to the end of the expanded variable to avoid
 * infinite loops in cases where the expanded value contains another `$`.
 * 
 * @param str Pointer to the string being processed.
 * @param i Pointer to the current index in the string.
 * @param len Pointer to the length of the string.
 * @param msl Pointer to the main structure of the program.
 */
void	dollar_expansion2(char **str, int *i, size_t *len, t_msl *msl)
{
	char			*dolim;
	unsigned char	*idx;

	idx = (unsigned char *)(*str);
	dolim = msl->parsing_utils->dollar_lim;
	if (dolim[idx[*i + 1]] >= 7 && dolim[idx[*i + 1]] <= 8)
		replace_dollar(str, i, len, msl);
	else if (dolim[idx[*i + 1]] == 9)
	{
		ft_memmove(&((*str)[*i]), &((*str)[*i + 2]), *len - *i);
		*i = *i + 2;
		*len -= 2;
	}
	else if (dolim[idx[*i + 1]] == 10)
		expand_vars(str, i, len, msl);
}

/**
 * @brief Replaces special `$` variables with their values.
 * 
 * This function replaces special `$` variables with their corresponding values:
 * - `$$`: Replaced with the shell's process ID.
 * - `$!`: Replaced with the last background process ID.
 * - `$?`: Replaced with the exit status of the last command.
 * - `$0`: Replaced with the name of the interpreter.
 * 
 * The replacement string is appended to the result, and the original `$` is removed.
 * The index is always moved to the end of the expanded variable to avoid
 * infinite loops in cases where the expanded value contains another `$`.
 * 
 * @param str Pointer to the string being processed.
 * @param i Pointer to the current index in the string.
 * @param len Pointer to the length of the string.
 * @param msl Pointer to the main structure of the program.
 */
void	replace_dollar(char **str, int *i, size_t *len, t_msl *msl)
{
	char	*replace;

	replace = NULL;
	if ((*str)[*i + 1] == '$')
		replace = ft_itoa(msl->msl_pid);
	else if ((*str)[*i + 1] == '!')
		replace = ft_itoa(msl->last_process);
	else if ((*str)[*i + 1] == '?')
		replace = ft_itoa(msl->exit_status);
	else if ((*str)[*i + 1] == '0')
		replace = ft_strdup(INTERPRETER_NAME);
	else
		*len = *len - 2;
	concatenate_strings(str, i, len, replace);
}

/**
 * @brief Expands environment variables in a string.
 * 
 * This function expands variables of the form `$VAR` by searching for their
 * values in the environment. If the variable exists, its value is appended
 * to the result. If it does not exist, the `$VAR` is removed from the string.
 * 
 * - The variable name is extracted using the `dollar_lim` dictionary.
 * - If the variable exists, its value is appended using `concatenate_strings2`.
 * - If the variable does not exist, the `$VAR` is removed from the string.
 *  The index is always moved to the end of the expanded variable to avoid
 * infinite loops in cases where the expanded value contains another `$`.
 * 
 * @param str Pointer to the string being processed.
 * @param i Pointer to the current index in the string.
 * @param len Pointer to the length of the string.
 * @param msl Pointer to the main structure of the program.
 */
void	expand_vars(char **str, int *i, size_t *len, t_msl *msl)
{
	t_env	*env_node;
	char	tmp;
	size_t	i_tmp;
	char	*new;

	i_tmp = ++(*i);
	while (msl->parsing_utils->dollar_lim[(unsigned char)(*str)[i_tmp]] >= 8)
		i_tmp++;
	tmp = (*str)[i_tmp];
	(*str)[i_tmp] = '\0';
	env_node = search_id_node(msl, &((*str)[*i]));
	(*str)[i_tmp] = tmp;
	(*str)[--(*i)] = '\0';
	*len = *len - (i_tmp - *i);
	if (env_node == NULL)
	{
		new = ft_strjoin(*str, &((*str)[i_tmp]));
		free(*str);
		*str = new;
	}
	else
	{
		*i = i_tmp;
		concatenate_strings2(str, i, len, env_node->value);
	}
}
