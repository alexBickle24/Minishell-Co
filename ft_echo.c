/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:22:09 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/04 23:21:25 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_args(t_pcmds *args)
{
	while (args)
	{
		ft_putstr_fd(args->cmd, 1);
		if (args->next)
			ft_putchar_fd(' ', 1);
		args = args->next;
	}
}

int	ft_echo(t_tocken *tocken)
{
	t_pcmds	*tmp;

	tmp = tocken->pcmds;
	if (ft_argscounter(tmp) == 1)
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	tmp = tmp->next;
	if (!ft_strncmp(tmp->cmd, "-n\0", 3))
		print_args(tmp);
	else
	{
		print_args(tmp);
		ft_putchar_fd('\n', 1);
	}
	return (0);
}
