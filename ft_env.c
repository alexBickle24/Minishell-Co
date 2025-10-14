/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 19:06:00 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:05:44 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    env:
    Primero vemos cuantos argumentos hay
    Si tod es correcto entonces vamos recorriendo los argumentos
    Cuando encontremos uno con _ lo hardcodeamos
    Ya que es el ultimo codigo escrito
*/

int	ft_env(t_msl *msl, t_pcmds *pcmds)
{
	t_env	*tmp;

	if (ft_argscounter(pcmds) >= 2)
		return (ft_putendl_fd("minishell: env: Too many Arguments", 2), 0);
	tmp = msl->own_env;
	if (tmp == NULL)
	{
		ft_putendl_fd("minishell: env: No such file or directory", 2);
		return (127);
	}
	while (tmp)
	{
		if (!ft_strncmp(tmp->id, "_\0", 2))
			ft_putendl_fd("_=/usr/bin/env", 1);
		else
		{
			ft_putstr_fd(tmp->id, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(tmp->value, 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
