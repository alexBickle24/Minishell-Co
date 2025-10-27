/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 23:43:51 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:51:37 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_signal = S_INIT;

/**
 * @brief Entry point of the program.
 * 
 * This function handles the execution modes of the shell:
 * - Interactive mode: If no arguments are passed.
 * - Command mode: If the `-c` flag is passed with a command.
 * 
 * It initializes the main structure (`t_msl`) and calls the appropriate
 * functions based on the arguments provided.
 * 
 * @param argc Number of arguments passed to the program.
 * @param argv Array of strings containing the arguments.
 * @param env Array of strings containing the environment variables.
 * @return Exit status of the shell.
 */
int	main(int argc, char **argv, char **env)
{
	t_msl	*msl;
	int		interpreter_exit;

	if (argc > 3)
		return (ft_errormini(), 1);
	if (argc == 1)
		interactive_mode(&msl, env);
	else if (argc == 3)
	{
		if (!ft_strncmp(argv[1], "-c\0", 3))
		{
			minishell_init(&msl, env, 1);
			evaluate_line(msl, (unsigned char *)argv[2]);
			interpreter_exit = msl->exit_status;
			return (free_msl(&msl), interpreter_exit);
		}
		else
		{
			ft_errorargs(argv[1]);
			return (free_msl(&msl), 2);
		}
	}
	return (free_msl(&msl), 0);
}

/**
 * @brief Evaluates a single line of input.
 * 
 * This function processes a cleaned input line by:
 * - Handling signals like SIGINT.
 * - Performing lexical and syntactical analysis.
 * - Expanding variables and preparing commands for execution.
 * - Executing the commands.
 * 
 * @param msl Pointer to the main structure of the program.
 * @param clean_line Cleaned input line to be evaluated.
 */
void	evaluate_line(t_msl *msl, unsigned char *clean_line)
{
	if (g_signal == S_SIGINT)
	{
		msl->exit_status = SIGINT + 128;
		g_signal = S_INIT;
	}
	lexer_parser(msl, clean_line);
	clean_expand_add_toexecuter(msl);
	executer(msl);
}

/**
 * @brief Implements the interactive mode of the shell.
 * 
 * This function initializes the shell environment, displays a custom prompt,
 * and continuously reads user input. It processes and evaluates commands
 * entered by the user, handles the command history, and cleans up resources
 * when exiting.
 * 
 * @param msl Double pointer to the main structure of the program.
 * @param env Array of strings containing the environment variables.
 */
void	interactive_mode(t_msl **msl, char **env)
{
	char	*line;

	reset_readlineoff();
	minishell_init(msl, env, 0);
	while (1)
	{
		if (g_signal == S_EXECUTION_S)
		{
			g_signal = S_INIT;
			write(1, "\n", 1);
		}
		set_ps1((*msl), (*msl)->sys);
		line = readline((*msl)->sys->ps1);
		add_history(line);
		(*msl)->clean_line = ft_strtrim(line, " \t\n\v\f\r");
		if (!(*msl)->clean_line)
		{
			ft_putstr_fd("exit\n", 2);
			free_line(line, *msl);
			break ;
		}
		evaluate_line ((*msl), (unsigned char *)(*msl)->clean_line);
		free_line(line, *msl);
	}
}

/**
 * @brief Frees memory allocated for the input line.
 * 
 * This function releases the memory allocated for the input line and the
 * cleaned line stored in the main structure.
 * 
 * @param line Input line to be freed.
 * @param msl Pointer to the main structure of the program.
 */
void	free_line(char *line, t_msl *msl)
{
	free(line);
	free(msl->clean_line);
	msl->clean_line = NULL;
}

/**
 * @brief Displays an initial message before starting the shell.
 * 
 * This function prompts the user to press "Enter" before starting the shell.
 * It is used to initialize the internal structures of the readline library
 * and ensure that the cursor pointer does not lose its reference due to
 * color codes in the prompt. Since we cannot use the readline library's
 * built-in function for this, we manually call `readline` here to achieve
 * the same effect.
 */
void	reset_readlineoff(void)
{
	char	*tmp;

	tmp = readline("PRESS ENTER TO START MINISHELL ...");
	free(tmp);
}
