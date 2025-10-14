/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:26:30 by alcarril          #+#    #+#             */
/*   Updated: 2025/01/06 01:54:30 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t		i;
	size_t		num_values;
	char		*ptr;

	num_values = ft_strlen(s);
	i = 0;
	ptr = (char *)malloc((num_values + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	while (i <= num_values)
	{
		ptr[i] = s[i];
		i++;
	}
	return (ptr);
}
