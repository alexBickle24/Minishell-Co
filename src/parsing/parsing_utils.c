/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:55:47 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/20 17:28:30 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
