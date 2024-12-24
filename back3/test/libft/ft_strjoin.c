/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:44:40 by sandriam          #+#    #+#             */
/*   Updated: 2024/10/07 07:45:31 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_s;
	int		i;
	int		j;

	new_s = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1));
	i = 0;
	j = 0;
	if (!new_s)
		return (0);
	while (s1[i] != '\0')
	{
		new_s[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		new_s[i + j] = s2[j];
		j++;
	}
	new_s[i + j] = '\0';
	return (new_s);
}
