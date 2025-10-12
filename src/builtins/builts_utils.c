/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builts_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicalons <vicalons@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:47:25 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/12 19:21:45 by vicalons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_argscounter(t_pcmds *pcmds)
{
	t_pcmds	*tmp;
	int		i;

	i = 0;
	tmp = pcmds;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
