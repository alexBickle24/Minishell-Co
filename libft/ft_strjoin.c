/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcarril <alcarril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:19:38 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/08 19:09:42 by alcarril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	j = 0;
	i = 0;
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *)malloc(total_len);
	if (!ptr)
		return (NULL);
	while ((i <= (total_len - 1)) && s1[i] != '\0')
	{
		ptr[i] = s1[i];
		i++;
	}
	while ((i < (total_len - 1)) && s2[j] != '\0')
		ptr[i++] = s2[j++];
	ptr[i] = '\0';
	return (ptr);
}
