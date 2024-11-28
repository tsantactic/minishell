/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 16:57:23 by sandriam          #+#    #+#             */
/*   Updated: 2024/10/07 07:44:54 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_size_n(int n)
{
	size_t	i;
	size_t	sign;

	i = 0;
	sign = 0;
	if (n < 0)
	{
		sign = 1;
		n = -n;
	}
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i + sign);
}

char	*ft_itoa(int n)
{
	size_t	i;
	char	*res;

	i = ft_size_n(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	res = (char *)malloc(i + 1);
	if (!res)
		return (0);
	res[i--] = '\0';
	if (n < 0)
	{
		res[0] = '-';
		n *= -1;
	}
	while (n != 0)
	{
		res[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
	return (res);
}
