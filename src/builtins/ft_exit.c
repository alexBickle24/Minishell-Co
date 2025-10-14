/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:30:44 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 02:41:34 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_msl *msl, t_tocken *tocken, char father)
{
	t_pcmds			*pcmds;
	int				num_args;
	int				exit_status;

	pcmds = tocken->pcmds;
	exit_status = msl->exit_status;
	num_args = ft_argscounter(pcmds);
	if (father)
		ft_putstr_fd("exit\n", 2);
	if (num_args > 2)
	{
		ft_putstr_fd("minishell: exit: Too much arguments\n", 2);
		free_msl(&msl);
		exit (1);
	}
	else if (num_args == 2)
		exit_onearg(&msl, pcmds);
	else
	{
		free_msl(&msl);
		exit (exit_status);
	}
}

void	exit_onearg(t_msl **msl, t_pcmds *pcmds)
{
	int	i;
	int	exit_code;

	i = 0;
	pcmds = pcmds->next;
	exit_code = ft_atoi(pcmds->cmd);
	while (pcmds->cmd[i] != '\0')
	{
		if (!ft_isdigit(pcmds->cmd[i]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			free_msl(msl);
			exit (2);
		}
		i++;
	}
	free_msl(msl);
	exit (exit_code);
}
