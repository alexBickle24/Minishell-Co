/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcarril <alcarril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:04:17 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/11 14:37:26 by alcarril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_trim_left(const char *s1, const char *set)
{
	size_t	count_left;
	size_t	i;
	size_t	j;

	count_left = 0;
	i = 0;
	while (s1[i] != '\0')
	{
		j = 0;
		while (set[j] != '\0')
		{
			if (set[j] == s1[i])
			{
				count_left++;
				break ;
			}
			j++;
		}
		if (set[j] == '\0')
			break ;
		i++;
	}
	return (count_left);
}

static size_t	ft_trim_right(const char *s1, const char *set)
{
	size_t	count_right;
	size_t	i;
	size_t	j;

	count_right = 0;
	i = ft_strlen(s1) - 1;
	while (i > 0)
	{
		j = 0;
		while (set[j] != '\0')
		{
			if (set[j] == s1[i])
			{
				count_right++;
				break ;
			}
			j++;
		}
		if (set[j] == '\0')
			break ;
		i--;
	}
	return (ft_strlen(s1) - count_right - 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i_left;
	size_t	i_right;
	size_t	s1_trim;

	if (!set || !s1)
		return (0);
	i_left = ft_trim_left(s1, set);
	i_right = ft_trim_right(s1, set);
	if (i_left > i_right)
		return (ft_strdup(""));
	s1_trim = i_right - i_left + 1;
	return (ft_substr(s1, i_left, s1_trim));
}
