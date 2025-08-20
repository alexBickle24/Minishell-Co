/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcarril <alcarril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:47:58 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/11 17:41:03 by alcarril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*dest_aux;
	char	*src_aux;

	if (!dest && !src)
		return (0);
	dest_aux = (char *)dest;
	src_aux = (char *)src;
	i = 0;
	while (i < n)
	{
		dest_aux[i] = src_aux[i];
		i++;
	}
	return (dest);
}
