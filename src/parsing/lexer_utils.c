/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:54:56 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:53:42 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Determines syntax errors during the lexicalization phase.
 * 
 * This function checks for syntax errors that occur during the lexicalization
 * phase of Bash, specifically errors that appear in the middle of the line.
 * It evaluates the current parsing state (`infstat`) and the type of the token
 * to identify unexpected tokens and report them.
 * 
 * - If the state is `INIT` or `REDIR` and the token is a pipe (`|`), it reports
 *   an unexpected pipe error.
 * - If the state is `OPERATOR` or `REDIR`, it checks for various token types
 *   (e.g., `<<`, `<`, `>`, `>>`, `|`) and reports the corresponding error.
 * 
 * This function ensures that lexical errors are caught early, mimicking the
 * behavior of Bash during its lexicalization phase.
 * 
 * @param type The type of the current token.
 * @param infstat The current parsing state.
 * @return Always returns 1 to indicate an error.
 */
int	get_unexpexted_errors1(int type, int infstat)
{
	if ((infstat == INIT || infstat == REDIR) && type == T_PIPE)
		ft_error_unexpect("|");
	else if (infstat == OPERATOR || infstat == REDIR)
	{
		if (type == T_HEREDOC)
			ft_error_unexpect("<<");
		if (type == T_HEREDOC_S)
			ft_error_unexpect("<<-");
		if (type == T_INFILE)
			ft_error_unexpect("<");
		if (type == T_OUTFILE)
			ft_error_unexpect(">");
		if (type == T_APPEND)
			ft_error_unexpect(">>");
		if (type == T_PIPE)
			ft_error_unexpect("|");
	}
	return (1);
}

/**
 * @brief Displays a syntax error message for an unexpected token.
 * 
 * This function prints an error message to the standard error output
 * indicating that a specific token was unexpected in the current context.
 * 
 * @param simbol The token that caused the syntax error.
 */
void	ft_error_unexpect(char *simbol)
{
	ft_putstr_fd("minishell: syntax error near unexpected token", 2);
	ft_putstr_fd(" `", 2);
	ft_putstr_fd(simbol, 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd("\n", 2);
}

/**
 * @brief Creates a new lexical token node.
 * 
 * This function allocates memory for a new `t_lex` node, initializes its
 * fields with the provided token type and raw string, and duplicates the
 * raw string to create the `str` field for further processing.
 * 
 * The `raw` field stores the original token string, while the `str` field
 * is used for variable expansion and error handling. Both are preserved
 * to handle ambiguous redirection errors, which are identified during the
 * phase where tokens are added to the execution structure. This behavior
 * mimics Bash's handling of ambiguous redirections during its parsing and
 * variable expansion phases.
 * 
 * @param type The type of the token (e.g., `T_PIPE`, `T_HEREDOC`).
 * @param raw The raw string representing the token.
 * @return A pointer to the newly created `t_lex` node, or `NULL` if memory
 *         allocation fails.
 */
t_lex	*lex_newnode(int type, char *raw)
{
	t_lex	*node;

	node = (t_lex *)malloc(sizeof(t_lex));
	if (node == NULL)
		return (NULL);
	node->raw = raw;
	node->len = ft_strlen(raw);
	node->type = type;
	node->next = NULL;
	node->str = ft_strdup(raw);
	return (node);
}

/**
 * @brief Adds a new lexical token node to the end of the lexer list.
 * 
 * This function appends a new `t_lex` node to the end of the linked list
 * of tokens in the `msl->lexer`. If the list is empty, the new node becomes
 * the head of the list.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param node Pointer to the `t_lex` node to be added.
 */
void	addback_lex(t_msl *msl, t_lex *node)
{
	t_lex	*current;

	current = msl->lexer;
	if (current == NULL)
	{
		msl->lexer = node;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = node;
}

/**
 * @brief Frees the memory allocated for the lexer list.
 * 
 * This function iterates through the linked list of tokens in `msl->lexer`
 * and frees the memory allocated for each node. If the token is a heredoc
 * (`T_HEREDOC`, `T_HEREDOC_S`, or `T_HERE_STR`), it also unlinks the
 * associated temporary file.
 * 
 * - If `all` is 1, it frees the `raw` and `str` fields of each token.
 * - Finally, it resets the `msl->lexer` pointer to `NULL` and calls
 *   `set_parsdefaultvals` to reset the parsing utilities.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param all If 1, frees the `raw` and `str` fields of each token.
 */
void	free_lexer(t_msl *msl, char all)
{
	t_lex	*lexer;
	t_lex	*tmp;

	lexer = msl->lexer;
	while (lexer)
	{
		if ((lexer->type == T_HEREDOC || lexer->type == T_HEREDOC_S
				|| lexer->type == T_HERE_STR) && lexer->str != NULL)
			unlink(lexer->str);
		if (all == 1)
		{
			if (lexer->raw != NULL)
				free (lexer->raw);
			if (lexer->str != NULL)
				free (lexer->str);
		}
		tmp = lexer->next;
		free(lexer);
		lexer = tmp;
	}
	msl->lexer = NULL;
	set_parsdefaultvals(msl);
}
