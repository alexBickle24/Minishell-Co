/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:47:58 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/05 21:00:55 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char		*ptr;
	size_t					i;

	ptr = (const unsigned char *)s;
	i = 1;
	while (i <= n)
	{
		if (*ptr == (unsigned char)c)
			return ((void *)ptr);
		i++;
		ptr++;
	}
	return (0);
}
