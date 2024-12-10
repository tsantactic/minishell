/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 12:03:01 by sandriam          #+#    #+#             */
/*   Updated: 2024/10/07 07:45:40 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_test_is_null(char *res, size_t i, size_t j)
{
	if (res)
		i = i - j + 1;
	else
		i++;
	return (i);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	j;
	size_t	i;
	char	*res;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	if (*big == '\0' || len == 0)
		return (0);
	while (i < len && big[i] != '\0')
	{
		j = 0;
		res = (char *)(big + i);
		while (i < len && little[j] != '\0' && little[j] == big[i])
		{
			i++;
			j++;
		}
		if (little[j] == '\0')
			return (res);
		i = ft_test_is_null(res, i, j);
	}
	return (0);
}
