/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 16:17:22 by sandriam          #+#    #+#             */
/*   Updated: 2024/10/07 07:45:45 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*s_new;
	size_t	i;
	size_t	j;

	i = 0;
	j = start;
	if (!s)
		return (0);
	if (start > ft_strlen(s))
		len = 0;
	if (len > (ft_strlen(s) - start))
		len = ft_strlen(s) - start;
	s_new = (char *)malloc((len + 1) * sizeof(char));
	if (!s_new)
		return (0);
	while (j < ft_strlen(s) && i < len)
	{
		s_new[i] = s[j];
		i++;
		j++;
	}
	s_new[i] = '\0';
	return (s_new);
}
