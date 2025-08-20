/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcarril <alcarril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:20:56 by alcarril          #+#    #+#             */
/*   Updated: 2024/10/15 19:22:11 by alcarril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

static	char	*ft_is_min(int num)
{
	unsigned int	j;
	char			*ptr;
	char			rest;

	j = 10;
	if (num != -2147483648)
		return (NULL);
	ptr = (char *)malloc(12);
	if (!(ptr))
		return (0);
	ptr[11] = '\0';
	while (num != 0)
	{
		rest = (num % 10) * -1 + '0';
		ptr[j] = rest;
		num = num / 10;
		j--;
	}
	ptr[0] = '-';
	return (ptr);
}

static unsigned char	ft_is_negative(int *num)
{
	if (*num < 0)
	{
		*num = *num * (-1);
		return (1);
	}
	return (0);
}

static unsigned int	ft_uint_len(int num)
{
	unsigned int	j;

	j = 1;
	while (num >= 10)
	{
		num = num / 10;
		j++;
	}
	return (j);
}

char	*ft_itoa(int n)
{
	unsigned char	i;
	unsigned int	j;
	char			*ptr;
	char			rest;

	ptr = ft_is_min(n);
	if (ptr)
		return (ptr);
	i = ft_is_negative(&n);
	j = ft_uint_len(n);
	ptr = (char *)malloc(i + j + 1);
	if (!(ptr))
		return (0);
	ptr[j + i] = '\0';
	while (n >= 10)
	{
		rest = (n % 10) + '0';
		ptr[j-- + i - 1] = rest;
		n = n / 10;
	}
	ptr[0 + i] = n + '0';
	if (i)
		ptr[0] = '-';
	return (ptr);
}
