/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcarril <alcarril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:00:20 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/08 23:04:44 by alcarril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	i;

	i = 0;
	ptr = (char *)malloc((size * nmemb));
	if (!(ptr))
		return (NULL);
	while (i < (nmemb * size))
	{
		*(char *)(ptr + i) = 0;
		i++;
	}
	return (ptr);
}
