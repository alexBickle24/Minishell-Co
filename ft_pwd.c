/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 19:06:14 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:05:57 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_freeptr(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

int	ft_pwd(t_msl *msl)
{
	ft_putendl_fd(msl->sys->pwd, 1);
	return (0);
}
