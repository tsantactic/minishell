/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:32:29 by sandriam          #+#    #+#             */
/*   Updated: 2024/10/07 07:45:32 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	res_d;
	size_t	res_s;

	i = 0;
	j = 0;
	res_d = ft_strlen(dst);
	res_s = ft_strlen(src);
	if (size <= res_d)
		return (res_s + size);
	while (dst[i] && i < size - 1)
		i++;
	while (src[j] && i < (size - 1))
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (res_d + res_s);
}
