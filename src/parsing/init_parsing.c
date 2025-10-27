/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:56:46 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:52:54 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Initializes the separator and operator dictionary.
 * 
 * This function sets up a dictionary (`sep_op`) that categorizes characters
 * as separators, operators, or other special symbols used during parsing.
 * Each character is assigned a code:
 * - 1: Whitespace characters (e.g., space, tab, newline).
 * - 2: Output redirection (`>`).
 * - 3: Input redirection (`<`).
 * - 4: Pipe operator (`|`).
 * - 5: Quotation marks (`"` and `'`).
 * 
 * @note This modular approach allows for easy extension of parsing rules
 * in the future, making the shell more scalable and maintainable.
 * 
 * @param sep_op Pointer to the dictionary of unsigned characters to be initialized.
 */
void	init_sep_op(unsigned char *sep_op)
{
	sep_op['\0'] = 1;
	sep_op['\t'] = 1;
	sep_op['\n'] = 1;
	sep_op['\v'] = 1;
	sep_op['\f'] = 1;
	sep_op['\r'] = 1;
	sep_op[' '] = 1;
	sep_op['>'] = 2;
	sep_op['<'] = 3;
	sep_op['|'] = 4;
	sep_op['\"'] = 5;
	sep_op['\''] = 5;
}

/**
 * @brief Initializes the dollar limits dictionary.
 * 
 * This function sets up a dictionary (`dollar_limits`) that defines the valid
 * characters that can follow a `$` during variable expansion. It uses the
 * `sep_op` dictionary to determine which characters are separators or operators.
 * 
 * The codes assigned to characters in `dollar_limits` are:
 * - Codes >= 8: Characters that can form part of environment variable keys.
 * - Codes >= 7: Characters used for direct expansion (e.g., `$?`, `$$`, `$!`).
 * - Code 6 or 0: Characters that expand but do not have a defined behavior,
 *   so they are printed alongside the `$`.
 * - Codes < 8: Characters considered as limits for environment variable names.
 * 
 * @note This design makes the parsing of `$` expansions modular and extensible,
 * allowing for future enhancements to variable handling in the shell.
 * 
 * @param dollar_limits Pointer to the dictionary of characters to be initialized.
 * @param sep_op Pointer to the separator and operator dictionary.
 */
void	init_dollar_lim(char *dollar_limits, unsigned char *sep_op)
{
	int	i;
	int	max;

	i = -1;
	max = 255;
	while (++i <= max)
	{
		if (sep_op[i] != 0)
			dollar_limits[i] = sep_op[i];
		if (i >= '1' && i <= '9')
			dollar_limits[i] = 9;
		if (ft_isalpha(i) || i == '_')
			dollar_limits[i] = 10;
	}
	dollar_limits['='] = 6;
	dollar_limits['?'] = 7;
	dollar_limits['$'] = 7;
	dollar_limits['!'] = 7;
	dollar_limits['0'] = 8;
}

/**
 * @brief Initializes the jump table for lexical analysis.
 * 
 * This function sets up a jump table (`f`) that maps parsing states to
 * their corresponding handler functions. Each function in the table
 * processes a specific type of token or character:
 * - `info`: Handles general information tokens.
 * - `spaces`: Handles whitespace characters.
 * - `redir_out`: Handles output redirection (`>`).
 * - `redir_in`: Handles input redirection (`<`).
 * - `pipe_op`: Handles the pipe operator (`|`).
 * - `quotes`: Handles quotation marks (`"` and `'`).
 * 
 * @note The use of a jump table makes the parsing process modular and
 * scalable, as new parsing states can be added easily by defining
 * additional handler functions.
 * 
 * @param f Pointer to the array of function pointers to be initialized.
 */
void	init_jump_table(int (**f)(t_msl *, int *, unsigned char *, t_parsing *))
{
	f[0] = info;
	f[1] = spaces;
	f[2] = redir_out;
	f[3] = redir_in;
	f[4] = pipe_op;
	f[5] = quotes;
}

/**
 * @brief Initializes the parsing utilities structure.
 * 
 * This function allocates and initializes the `t_parsing` structure
 * used for parsing. It sets up the separator/operator dictionary,
 * the dollar limits dictionary, and the lexical analysis jump table.
 * 
 * @note By centralizing the initialization of parsing utilities, this
 * function ensures that the parsing process is modular and can be
 * extended easily in the future.
 * 
 * @param msl Pointer to the main structure of the program.
 * @return Pointer to the initialized `t_parsing` structure, or `NULL` on failure.
 */
t_parsing	*init_parsing(t_msl *msl)
{
	t_parsing	*pars;

	if (!msl)
		return (NULL);
	pars = NULL;
	if (msl->parsing_utils == NULL)
	{
		pars = (t_parsing *)ft_calloc(sizeof(t_parsing), 1);
		if (pars == NULL)
			return (NULL);
		init_sep_op(pars->sep_op);
		init_dollar_lim(pars->dollar_lim, pars->sep_op);
		init_jump_table(pars->lex);
	}
	return (pars);
}

/**
 * @brief Sets default values for the parsing utilities structure.
 * 
 * This function resets the parsing status fields in the `parsing_utils`
 * structure to their default values, preparing it for a new parsing operation.
 * 
 * @note This function ensures that the parsing utilities are reusable
 * and maintainable, contributing to the modularity of the parsing process.
 * 
 * @param msl Pointer to the main structure of the program.
 */
void	set_parsdefaultvals(t_msl *msl)
{
	msl->parsing_utils->infstat = 0;
	msl->parsing_utils->lexstat = 0;
	msl->parsing_utils->parstat = 0;
	msl->parsing_utils->ptr = 0;
}
