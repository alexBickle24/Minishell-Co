/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:48:27 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:04:57 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd(t_tocken *c_tocken)
{
	int		error;
	t_pcmds	*pcmds;

	pcmds = c_tocken->pcmds;
	error = c_tocken->error_cmd;
	if (error == 1)
		ft_exterror_exes(c_tocken->cmd_tb[0], 0);
	else if (error == 2)
		ft_exterror_cmd(c_tocken->cmd_tb[0]);
	else if (error == 3)
		ft_exterror_exes(c_tocken->cmd_tb[0], 1);
	else if (error == 0)
	{
		if (execve(pcmds->cmd, c_tocken->cmd_tb, c_tocken->env_tb) == -1)
			ft_exterrno();
		else
			exit(0);
	}
}

int	is_builtin(t_tocken *tocken)
{
	if (!tocken->pcmds)
		return (0);
	if (!ft_strncmp(tocken->pcmds->cmd, "echo\0", 5))
		return (1);
	if (!ft_strncmp(tocken->pcmds->cmd, "cd\0", 3))
		return (2);
	if (!ft_strncmp(tocken->pcmds->cmd, "pwd\0", 4))
		return (3);
	if (!ft_strncmp(tocken->pcmds->cmd, "export\0", 7))
		return (4);
	if (!ft_strncmp(tocken->pcmds->cmd, "unset\0", 6))
		return (5);
	if (!ft_strncmp(tocken->pcmds->cmd, "env\0", 4))
		return (6);
	if (!ft_strncmp(tocken->pcmds->cmd, "exit\0", 5))
		return (7);
	return (0);
}

int	father_builtin(t_msl *msl, t_tocken *c_tocken, int builtin)
{
	if (builtin == 1)
		return (ft_echo(c_tocken));
	else if (builtin == 2)
		return (ft_cd(msl, c_tocken->pcmds));
	else if (builtin == 3)
		return (ft_pwd(msl));
	else if (builtin == 4)
		return (ft_export(msl, c_tocken->pcmds));
	else if (builtin == 5)
		return (ft_unset(msl, c_tocken));
	else if (builtin == 6)
		return (ft_env(msl, c_tocken->pcmds));
	else if (builtin == 7)
		ft_exit(msl, c_tocken, 1);
	return (0);
}

void	create_pipes(t_tocken *c_tocken)
{
	if (pipe(c_tocken->pipe_fds) == -1)
		ft_errerrno();
}

void	cmd_vs_builtin(t_msl *msl, t_tocken *c_tocken, int builtin)
{
	if (builtin == 1)
		exit (ft_echo(c_tocken));
	else if (builtin == 2)
		exit (ft_cd(msl, c_tocken->pcmds));
	else if (builtin == 3)
		exit (ft_pwd(msl));
	else if (builtin == 4)
		exit(ft_export(msl, c_tocken->pcmds));
	else if (builtin == 5)
		exit (ft_unset(msl, c_tocken));
	else if (builtin == 6)
		exit (ft_env(msl, c_tocken->pcmds));
	else if (builtin == 7)
		ft_exit (msl, c_tocken, 0);
	else
		exec_cmd(c_tocken);
}
