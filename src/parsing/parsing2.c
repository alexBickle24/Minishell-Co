/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:43:16 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/20 18:44:55 by alejandro        ###   ########.fr       */
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

//segundo recorrido, compuebo si despues de la expansion habia espacios
// //y ademas limpio comillas
// void	adding_cmds(t_msl *msl, t_tocken *current, t_lex *lex)//FUNCOINA
// {
// 	char  *start;
// 	char *str;
// 	char jump;

// 	vars_interpolation(&(lex->str), msl, (&lex->len));
// 	str = lex->str;
// 	start = str;
// 	if (*str == '\0')//para manejo de argumento vacio sin comillas
// 	{
// 		free(lex->raw);
// 		free(lex->str);
// 		return ;
// 	}
// 	while(*str)
// 	{
// 		jump = check_clean_quotes(msl, str, 1);
// 		if (is_space(*str) && msl->parsing_utils->lexstat == NO_QUOTES)
// 		{
// 			*str = '\0';
// 			list_addback_pcmds(&(current->pcmds), list_new_pcmds(ft_strdup(start)));
// 			str++;
// 			jump_separator(&str);
// 			start = str;
// 		}
// 		else if (jump == 1)
// 			str++;
// 	}
// 	list_addback_pcmds(&(current->pcmds), list_new_pcmds(ft_strdup(start)));
// 	free(lex->raw);
// 	free(lex->str);
// }

// void	adding_files(t_msl *msl, t_tocken *current, t_lex *lex)//
// {
// 	char ambiguos;
// 	char *str;
// 	char jump;

// 	vars_interpolation(&(lex->str), msl, (&lex->len));
// 	str = lex->str;
// 	if (*str == '\0')//para manejo de redicreccion vacia sin comillas
// 		ambiguos = 1;
// 	else
// 		ambiguos = 0;
// 	while(*str && ambiguos == 0)
// 	{
// 		jump = check_clean_quotes(msl, str, 1);
// 		if (is_space(*str) && msl->parsing_utils->lexstat == NO_QUOTES)
// 			ambiguos = 1;
// 		if (jump == 1)
// 			str++;
// 	}
// 	if (ambiguos == 1)
// 	{
// 		list_addback_infiles(&(current->files), list_new_files(lex->raw, lex->type, ambiguos));
// 		free(lex->str);
// 	}
// 	else
// 	{
// 		list_addback_infiles(&(current->files), list_new_files(lex->str, lex->type, ambiguos));
// 		free(lex->raw);
// 	}
// }