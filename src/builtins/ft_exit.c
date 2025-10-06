/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:30:44 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/06 15:06:40 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_msl *msl, t_tocken *tocken, char father)
{
	t_pcmds	*pcmds;

	pcmds = tocken->pcmds;
	if (ft_argscounter(pcmds) > 1)
	{
		ft_putstr_fd("minishell: exit: Too much arguments\n", 2);
		exit (1);
	}
	if (father)
	{
		ft_putstr_fd("exit\n", 2);
		free_msl(&msl);
	}
	exit (msl->exit_status);
}
