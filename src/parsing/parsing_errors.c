/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:55:34 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:54:04 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Handles parsing errors related to unexpected newlines.
 * 
 * This function checks if a parsing error (`pars_err`) has been flagged due
 * to an unexpected newline. If so, it processes all heredocs in the lexer
 * list to create temporary files, displays an error message, and resets the
 * parsing error flag.
 * 
 * This behavior mimics how Bash handles errors at the end of a line during
 * the parsing phase. Bash processes the list of lexed tokens, creates the
 * heredocs even if there is an error, and then displays the error message.
 * 
 * - For each heredoc (`T_HEREDOC` or `T_HEREDOC_S`), it calls
 *   `create_heredoc_nwlerr` to create the temporary file.
 * - Calls `new_line_err` to display the error message and clean up resources.
 * - Resets `msl->pars_err` to 0 after handling the error.
 * 
 * @param msl Pointer to the main structure of the program.
 * @return 1 if a parsing error was handled, 0 otherwise.
 */
char	check_nwl_error(t_msl *msl)
{
	t_lex	*lexer;

	if (msl->pars_err == 1)
	{
		lexer = msl->lexer;
		while (lexer)
		{
			if (lexer->type == T_HEREDOC || lexer->type == T_HEREDOC_S)
				create_heredoc_nwlerr(msl, lexer);
			lexer = lexer->next;
		}
		new_line_err(msl);
		msl->pars_err = 0;
		return (1);
	}
	return (0);
}

/**
 * @brief Displays an error message for unexpected newlines and cleans up resources.
 * 
 * This function is called when a parsing error is flagged due to an unexpected
 * newline. It displays the error message, frees the lexer and token structures,
 * and returns an error flag.
 * 
 * This behavior mimics how Bash handles errors at the end of a line during
 * the parsing phase. Bash processes the list of lexed tokens, creates the
 * heredocs even if there is an error, and then displays the error message.
 * 
 * - Displays the error message defined by `NEWLINE_ERR`.
 * - Frees the lexer list using `free_lexer`.
 * - Frees the token list using `free_tockens`.
 * 
 * @param msl Pointer to the main structure of the program.
 * @return 1 if an error was handled, 0 otherwise.
 */
char	new_line_err(t_msl *msl)
{
	char	error;

	error = 0;
	if (msl->pars_err == 1)
	{
		error = 1;
		ft_putstr_fd(NEWLINE_ERR, 2);
		free_lexer(msl, 1);
		free_tockens(msl);
	}
	return (error);
}

/**
 * @brief Creates temporary files for heredocs during a newline error.
 * 
 * This function processes a lexer node representing a heredoc (`T_HEREDOC`
 * or `T_HEREDOC_S`) during a parsing error caused by an unexpected newline.
 * It creates the temporary file for the heredoc and updates the `str` field
 * of the lexer node with the file name.
 * 
 * This behavior mimics how Bash handles heredocs during parsing errors. Bash
 * creates the heredocs even if there is an error at the end of the line, and
 * this function replicates that behavior.
 * 
 * - For `T_HEREDOC`: Calls `create_heredoc` with `sangria = 0`.
 * - For `T_HEREDOC_S`: Calls `create_heredoc` with `sangria = 1`.
 * - Frees the previous `str` field before updating it.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param lex Pointer to the lexer node representing the heredoc.
 */
void	create_heredoc_nwlerr(t_msl *msl, t_lex *lex)
{
	free(lex->str);
	if (lex->type == T_HEREDOC)
		lex->str = create_heredoc(msl, lex->raw, 0);
	else if (lex->type == T_HEREDOC_S)
		lex->str = create_heredoc(msl, lex->raw, 1);
}

/**
 * @brief Adds a new token node to the end of the token list.
 * 
 * This function appends a new token node to the end of the token list. If the
 * list is empty, the new node becomes the head of the list.
 * 
 * - If the list is empty, the new node is assigned as the head.
 * - Otherwise, the function iterates to the end of the list and appends the
 *   new node.
 * 
 * @param list Pointer to the pointer of the token list.
 * @param new_node Pointer to the new token node to be added.
 */
void	list_addback_tocken(t_tocken **list, t_tocken *new_node)
{
	t_tocken	*current;

	if (!new_node)
		return ;
	current = *list;
	if (!*list)
		*list = new_node;
	else
	{
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}
