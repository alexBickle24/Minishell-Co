/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:29:58 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:54:41 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Bridges parsing and execution by preparing tokens for the executor.
 * 
 * This function represents the transition phase between parsing and execution.
 * It processes the lexer list, expanding variables, handling special cases
 * (e.g., heredocs, files, commands), and populating the executor structure
 * (`msl->tocken`) with the parsed information. This step ensures that the
 * parsed data is correctly structured and ready for execution by the shell core.
 * 
 * - If the lexer list is empty due to an error, if a `SIGINT` signal is received,
 *   or if there is an error at the end of the line, the execution is interrupted.
 * - Handles different token types:
 *   - `T_PIPE`: Creates a new execution token.
 *   - `T_CMD`: Processes and adds commands to the current token.
 *   - `T_HEREDOC` to `T_HERE_STR`: Handles heredocs and herestrings.
 *   - File redirections: Adds file information to the current token.
 * - Frees the lexer nodes after processing to avoid memory leaks.
 * 
 * @param msl Pointer to the main structure of the program.
 */
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

/**
 * @brief Adds commands to the current token.
 * 
 * This function processes a lexer node of type `T_CMD`, expanding variables
 * and handling the `~` character for home directory expansion. It splits the
 * command string into parts and adds them to the `pcmds` list of the current token.
 * 
 * - Calls `home_case` to expand `~` to the home directory.
 * - Calls `vars_interpolation` to expand variables in the command string.
 * - Splits the command string into parts using `loop_cmd_split` and adds
 *   them to the `pcmds` list.
 * - Frees the `raw` and `str` fields of the lexer node after processing.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param current Pointer to the current token being processed.
 * @param lex Pointer to the lexer node containing the command.
 */
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

/**
 * @brief Creates a new token and links it to the token list.
 * 
 * This function processes a lexer node of type `T_PIPE` and creates a new
 * token in the executor structure. The new token is linked to the current
 * token list, and the total token count is incremented.
 * 
 * - Increments the total token count (`msl->total_tockens`).
 * - Creates a new token using `list_new_tocken` and links it to the current token.
 * - Frees the `raw` and `str` fields of the lexer node after processing.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param current Pointer to the pointer of the current token being processed.
 * @param lexer Pointer to the lexer node containing the pipe operator.
 */
void	adding_tocken(t_msl *msl, t_tocken **current, t_lex *lexer)
{
	msl->total_tockens++;
	(*current)->next = list_new_tocken(msl->total_tockens);
	(*current) = (*current)->next;
	free(lexer->raw);
	free(lexer->str);
}

/**
 * @brief Adds file redirections to the current token.
 * 
 * This function processes a lexer node representing a file redirection
 * (e.g., `<`, `>`, `>>`). It expands variables and handles ambiguous file
 * errors. The file is added to the `files` list of the current token.
 * 
 * - Calls `home_case` to expand `~` to the home directory.
 * - Calls `vars_interpolation` to expand variables in the file name.
 * - Checks for ambiguous file errors using `check_ambiguos_file`:
 *   - If ambiguous, the `raw` field is added to the `files` list.
 *   - Otherwise, the `str` field is added.
 * - Frees the unused field (`raw` or `str`) after processing.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param current Pointer to the current token being processed.
 * @param lex Pointer to the lexer node containing the file redirection.
 */
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

/**
 * @brief Adds heredocs and herestrings to the current token.
 * 
 * This function processes a lexer node representing a heredoc (`<<`, `<<-`)
 * or herestring (`<<<`). It creates the appropriate temporary file for the
 * heredoc or herestring and adds it to the `files` list of the current token.
 * 
 * - For `T_HEREDOC`: Calls `create_heredoc` with `sangria = 0`.
 * - For `T_HEREDOC_S`: Calls `create_heredoc` with `sangria = 1`.
 * - For `T_HERE_STR`: Calls `create_here_str`.
 * - Creates a new file node using `list_new_files` and adds it to the `files` list.
 * - Frees the `raw` field of the lexer node after processing.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param current Pointer to the current token being processed.
 * @param lex Pointer to the lexer node containing the heredoc or herestring.
 */
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
