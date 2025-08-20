/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcarril <alcarril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:03:51 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/11 14:39:26 by alcarril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t n)
{
	size_t	i;
	size_t	j;

	if (!*to_find)
		return ((char *)str);
	i = 0;
	while (str[i] && i < n)
	{
		j = 0;
		while (str[i + j] == to_find[j] && i + j < n)
		{
			if (!to_find[j + 1])
				return ((char *)str + i);
			j++;
		}
		i++;
	}
	return (0);
}
