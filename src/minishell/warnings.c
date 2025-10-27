/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   warnings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 01:30:38 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:59:37 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Displays a warning when the shell level (`SHLVL`) is too high.
 * 
 * This function prints a warning message to `stderr` indicating that the
 * shell level is too high and will be reset to 1.
 * 
 * @param num The current shell level (`SHLVL`) value.
 */
void	ft_shlvl_warning(int num)
{
	char	*str;

	str = ft_itoa(num);
	ft_putstr_fd("minishell: warning: shell level ", 2);
	ft_putstr_fd("(", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(") too high, resetting to 1", 2);
	free(str);
}

/**
 * @brief Displays a message about running commands as an administrator.
 * 
 * This function prints a message to `stderr` advising the user to use `sudo`
 * to run commands as the root user.
 */
void	ft_error_home(void)
{
	ft_putstr_fd("To run a command as administrator (user \"root\")", 2);
	ft_putstr_fd(", use \"sudo <command>\".\n", 2);
	ft_putstr_fd("See \"man sudo_root\" for details.\n\n", 2);
}

/**
 * @brief Displays an error message for invalid command-line arguments.
 * 
 * This function prints an error message to `stderr` indicating that the
 * provided argument is not valid and suggests using the `-c` option.
 * 
 * @param arg The invalid argument provided by the user.
 */
void	ft_errorargs(char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": not a valid option,", 2);
	ft_putstr_fd(" use <\"-c\" \"prompt\">\n", 2);
}

/**
 * @brief Displays an error message for too many arguments.
 * 
 * This function prints an error message to `stderr` indicating that too
 * many arguments were provided to `minishell`.
 */
void	ft_errormini(void)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("to much arguments\n", 2);
}
