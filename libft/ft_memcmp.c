/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:56:04 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/05 18:35:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*first;
	const unsigned char	*second;

	first = (unsigned char *)s1;
	second = (unsigned char *)s2;
	i = 1;
	while (i <= n)
	{
		if (*first != *second)
			return (*first - *second);
		i++;
		first++;
		second++;
	}
	return (0);
}
