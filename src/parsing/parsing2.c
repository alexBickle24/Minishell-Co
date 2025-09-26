/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:43:16 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/26 22:58:05 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
