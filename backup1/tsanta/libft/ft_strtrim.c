/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:48:09 by sandriam          #+#    #+#             */
/*   Updated: 2024/10/07 07:45:43 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	end;

	end = 0;
	if (!s1 || !set)
		return (0);
	while (*s1 && ft_strchr(set, *s1) != 0)
		s1++;
	end = ft_strlen(s1);
	while (end != 0 && ft_strchr(set, s1[end]) != 0)
		end--;
	return (ft_substr(s1, 0, end + 1));
}
