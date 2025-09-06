/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   warnings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 01:30:38 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/06 01:39:46 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_shlvl_warning(int num)
{
	char	*str;

	str = ft_itoa(num);
	ft_putstr_fd("minishell: warning: shell level ", 2);
	ft_putstr_fd("(", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(") too high, resetting to 1", 2);
	free(str);
}

void	ft_error_home(void)
{
	ft_putstr_fd("To run a command as administrator (user \"root\")", 2);
	ft_putstr_fd(", use \"sudo <command>\".\n", 2);
	ft_putstr_fd("See \"man sudo_root\" for details.\n\n", 2);
}
