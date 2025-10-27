/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:04:03 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:52:01 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Concatenates a replacement string into the original string.
 * 
 * This function replaces a portion of the original string with a replacement
 * string. It updates the string, its length, and the index to reflect the
 * changes. The replacement occurs at the current index, and the rest of the
 * string is preserved.
 * 
 * - The original string is split at the current index.
 * - The replacement string is inserted at the split point.
 * - The remaining part of the original string is appended after the replacement.
 * - The index is moved to the end of the replacement to avoid infinite loops
 *   if the replacement contains `$`.
 * 
 * @param str Pointer to the original string to be modified.
 * @param i Pointer to the current index in the string.
 * @param len Pointer to the length of the string.
 * @param replace The replacement string to be inserted.
 */
void	concatenate_strings(char **str, int *i, size_t *len, char *replace)
{
	size_t	replace_len;
	char	*tmp;
	char	*tmp_after;
	char	*new;

	replace_len = ft_strlen(replace);
	(*str)[*i] = '\0';
	tmp = ft_strjoin(*str, replace);
	tmp_after = &((*str)[*i + 2]);
	new = ft_strjoin(tmp, tmp_after);
	free(replace);
	free(tmp);
	free(*str);
	*str = new;
	*len = *len + replace_len;
	*i = *i + replace_len;
}

/**
 * @brief Concatenates a replacement string at the current index.
 * 
 * This function appends a replacement string to the original string at the
 * current index. It updates the string, its length, and the index to reflect
 * the changes. The replacement is appended, and the rest of the string is
 * preserved.
 * 
 * - The replacement string is appended to the original string.
 * - The remaining part of the original string is appended after the replacement.
 * - The index is updated to the end of the replacement to avoid infinite loops
 *   if the replacement contains `$`.
 * 
 * @param str Pointer to the original string to be modified.
 * @param i Pointer to the current index in the string.
 * @param len Pointer to the length of the string.
 * @param replace The replacement string to be appended.
 */
void	concatenate_strings2(char **str, int *i, size_t *len, char *replace)
{
	size_t	replace_len;
	char	*tmp;
	char	*new;

	replace_len = ft_strlen(replace);
	tmp = ft_strjoin(*str, replace);
	new = ft_strjoin(tmp, &(*str)[(*i)]);
	*i = ft_strlen(new) - ft_strlen(&(*str)[(*i)]);
	free(tmp);
	free(*str);
	*str = new;
	*len = *len + replace_len;
}

/**
 * @brief Handles the expansion of the `~` character to the user's home directory.
 * 
 * This function replaces the `~` character at the start of a string with the
 * user's home directory. If the `~` is followed by a `/`, the home directory
 * is concatenated with the rest of the string. If the `~` is standalone, it
 * is replaced entirely by the home directory.
 * 
 * - The function retrieves the `HOME` environment variable using `search_id_node`.
 * - If `HOME` is not set, it falls back to the system's default home directory.
 * - The string is updated to reflect the replacement.
 * 
 * @param str Pointer to the string to be modified.
 * @param msl Pointer to the main structure of the program.
 */
void	home_case(char **str, t_msl *msl)
{
	t_env	*env_node;
	char	*home;
	char	*tmp;

	if (!str || !*str)
		return ;
	env_node = search_id_node(msl, "HOME");
	if (!env_node)
		home = ft_strdup(msl->sys->home);
	else
		home = ft_strdup(env_node->value);
	if (str[0][0] == '~' && str[0][1] == '\0')
	{
		free(*str);
		*str = home;
	}
	else if (str[0][0] == '~' && str[0][1] == '/')
	{
		tmp = ft_strjoin(home, &(str[0][1]));
		free(*str);
		free(home);
		*str = tmp;
	}
	else
		free(home);
}
