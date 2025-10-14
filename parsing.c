/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:29:58 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:07:10 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Funcion para recorrer la lista de lexer e ir añadiedno a la estrcutura de 
//ejecucio las palabras segun su etiqueta (tipo sintactico).
//sepodria meter optimizacion para que deje de lexerizas si señal de sigint
// en heredoc
void	clean_expand_add_toexecuter(t_msl *msl)
{
	t_lex		*lexer;
	t_lex		*tmp;
	t_tocken	*current;

	lexer = msl->lexer;
	if (g_signal != S_INIT || lexer == NULL || check_nwl_error(msl))
		return ;
	msl->tocken = list_new_tocken(1);
	current = msl->tocken;
	msl->total_tockens = 1;
	while (lexer)
	{
		tmp = lexer->next;
		if (lexer->type == T_PIPE)
			adding_tocken(msl, &current, lexer);
		else if (lexer->type == T_CMD)
			adding_cmds(msl, current, lexer);
		else if (lexer->type >= T_HEREDOC && lexer->type <= T_HERE_STR)
			adding_here(msl, current, lexer);
		else
			adding_files(msl, current, lexer);
		free(lexer);
		lexer = tmp;
	}
	msl->lexer = NULL;
}

void	adding_cmds(t_msl *msl, t_tocken *current, t_lex *lex)
{
	char	*start;
	char	*str;

	home_case(&(lex->str), msl);
	vars_interpolation(&(lex->str), msl, (&lex->len));
	str = lex->str;
	start = str;
	if (*str == '\0')
	{
		free(lex->raw);
		free(lex->str);
		return ;
	}
	loop_cmd_split(&str, &start, msl, current);
	list_addback_pcmds(&(current->pcmds), list_new_pcmds(ft_strdup(start)));
	free(lex->raw);
	free(lex->str);
}

void	adding_tocken(t_msl *msl, t_tocken **current, t_lex *lexer)
{
	msl->total_tockens++;
	(*current)->next = list_new_tocken(msl->total_tockens);
	(*current) = (*current)->next;
	free(lexer->raw);
	free(lexer->str);
}

void	adding_files(t_msl *msl, t_tocken *current, t_lex *lex)
{
	char	ambiguos;

	home_case(&(lex->str), msl);
	vars_interpolation(&(lex->str), msl, (&lex->len));
	ambiguos = check_ambiguos_file(lex->str, msl);
	if (ambiguos == 1)
	{
		list_addback_infiles(&(current->files),
			list_new_files(lex->raw, lex->type, ambiguos));
		free(lex->str);
	}
	else
	{
		list_addback_infiles(&(current->files),
			list_new_files(lex->str, lex->type, ambiguos));
		free(lex->raw);
	}
}

void	adding_here(t_msl *msl, t_tocken *current, t_lex *lex)
{
	t_files	*node;

	free(lex->str);
	if (lex->type == T_HEREDOC)
		lex->str = create_heredoc(msl, lex->raw, 0);
	else if (lex->type == T_HEREDOC_S)
		lex->str = create_heredoc(msl, lex->raw, 1);
	else if (lex->type == T_HERE_STR)
		lex->str = create_here_str(lex->raw);
	node = list_new_files(lex->str, lex->type, 0);
	if (!node)
		return ;
	list_addback_infiles(&(current->files), node);
	free(lex->raw);
}
