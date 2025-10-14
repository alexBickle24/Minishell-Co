/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 23:43:51 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:31:25 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = S_INIT;

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

void	interactive_mode(t_msl **msl, char **env)
{
	char	*line;

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

void	free_line(char *line, t_msl *msl)
{
	free(line);
	free(msl->clean_line);
	msl->clean_line = NULL;
}
