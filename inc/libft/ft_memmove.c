/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcarril <alcarril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:36:51 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/11 15:35:37 by alcarril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!dest && !src)
		return (NULL);
	i = 1;
	if (src < dest)
	{
		while (i <= n)
		{
			*(char *)(dest + (n - i)) = *(char *)(src + (n - i));
			i++;
		}
		return (dest);
	}
	dest = ft_memcpy(dest, src, n);
	return (dest);
}
