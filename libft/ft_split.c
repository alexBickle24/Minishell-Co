/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 20:22:17 by alex              #+#    #+#             */
/*   Updated: 2024/10/14 23:01:10 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_count_words(const char *s, int c);

static size_t	*ft_counter_begin(const char *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	*aux_b;

	i = 0;
	j = 0;
	aux_b = malloc((ft_count_words(s, c)) * sizeof(size_t));
	if (!aux_b)
		return (NULL);
	while (s[i] != '\0')
	{
		if ((i == 0 && s[i] != c) || (i >= 1 && (s[i] != c && s[i - 1] == c)))
		{
			aux_b[j] = i;
			j++;
		}
		i++;
	}
	return (aux_b);
}

static size_t	*ft_counter_end(const char *s, char c)
{
	size_t	i;
	size_t	t;
	size_t	*aux_e;

	i = 0;
	t = 0;
	aux_e = malloc((ft_count_words(s, c)) * sizeof(size_t));
	if (!aux_e)
		return (NULL);
	while (s[i] != '\0')
	{
		if ((s[i] != c && s[i + 1] == s[ft_strlen(s)])
			|| ((s[i] != c && s[i + 1] == c)))
		{
			aux_e[t] = i;
			t++;
		}
		i++;
	}
	return (aux_e);
}

static unsigned int	ft_count_words(const char *s, int c)
{
	unsigned int	count_words;
	int				i;

	i = 0;
	count_words = 0;
	while (s[i])
	{
		if ((i == 0 && s[i] != c) || (i >= 1 && (s[i] != c && s[i - 1] == c)))
			count_words++;
		i++;
	}
	return (count_words);
}

static void	*ft_free(char **ptr, int i, size_t *aux_b, size_t *aux_e)
{
	while (i >= 0)
	{
		free(*(ptr + i));
		i--;
	}
	free(aux_b);
	free(aux_e);
	free(ptr);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	char	**ptr;
	size_t	*aux_b;
	size_t	*aux_e;
	size_t	n;

	if (!s)
		return (NULL);
	aux_b = ft_counter_begin(s, c);
	if (!aux_b)
		return (NULL);
	aux_e = ft_counter_end(s, c);
	if (!aux_e)
		return (free(aux_b), NULL);
	ptr = (char **)ft_calloc((ft_count_words(s, c) + 1), sizeof(char *));
	if (!ptr)
		return (free(aux_b), free(aux_e), NULL);
	n = -1;
	while (++n < ft_count_words(s, c))
	{
		*(ptr + n) = ft_substr(s, aux_b[n], (aux_e[n] - aux_b[n] + 1));
		if (!*(ptr + n))
			return (ft_free(ptr, n, aux_b, aux_e));
	}
	return (free(aux_b), free(aux_e), ptr);
}
