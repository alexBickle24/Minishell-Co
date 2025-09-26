/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:49:44 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/26 22:51:40 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	executer(t_msl *msl)
{
	char	*last_arg;

	last_arg = NULL;
	if (msl && g_signal == S_INIT && msl->tocken != NULL)
	{
		g_signal = S_EXECUTION;
		last_arg = set_last_arg(msl);
		if (msl->total_tockens == 1 && is_builtin(msl->tocken))
			only_builtin(msl);
		else
			execute_orders(msl);
	}
	if (g_signal != S_EXECUTION_S)
		g_signal = S_INIT;
	create_last_arg(msl, last_arg);
	msl->pars_err = 0;
	free_tockens(msl);
}

void	only_builtin(t_msl *msl)
{
	t_tocken	*c_tocken;

	c_tocken = msl->tocken;
	check_create_redirs(c_tocken, c_tocken->files);
	fordward_in(c_tocken);
	fordward_out(c_tocken);
	if (c_tocken->error_file != 1)
		msl->exit_status = father_builtin(msl, c_tocken, is_builtin(c_tocken));
	else
		msl->exit_status = 1;
}

void	execute_orders(t_msl *msl)
{
	t_tocken	*c_tocken;

	c_tocken = msl->tocken;
	while (c_tocken)
	{
		execute_childs(c_tocken, msl);
		c_tocken = c_tocken->next;
	}
	wait_childs3(msl);
}

void	execute_childs(t_tocken *c_tocken, t_msl *msl)
{
	pid_t	pid;
	int		s_builtin;

	s_builtin = is_builtin(c_tocken);
	check_create_redirs(c_tocken, c_tocken->files);
	if (!s_builtin)
		evaluate_tocken_cmds_errors(c_tocken, msl);
	if (msl->total_tockens - c_tocken->index != 0)
		create_pipes(c_tocken->next);
	pid = fork();
	if (pid < 0)
		ft_errerrno();
	if (pid == 0)
		child_process(s_builtin, c_tocken, msl);
	if (c_tocken->index != 1)
		close_fds(c_tocken->pipe_fds);
	c_tocken->pid = pid;
}

void	child_process(int s_builtin, t_tocken *c_tocken, t_msl *msl)
{
	fordward_in(c_tocken);
	fordward_out(c_tocken);
	signal_init_childs();
	if (c_tocken->error_file != 0)
		exit (1);
	if (c_tocken->pcmds)
		cmd_vs_builtin(msl, c_tocken, s_builtin);
	else
		exit(0);
}
