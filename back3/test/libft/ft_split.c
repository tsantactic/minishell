/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 12:13:17 by sandriam          #+#    #+#             */
/*   Updated: 2024/10/07 07:45:20 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_count(char const *s, char c, unsigned int size)
{
	unsigned int	j;
	unsigned int	count_word;

	j = 0;
	count_word = 0;
	while (j < size)
	{
		while (s[j] && s[j] == c)
			j++;
		if (s[j])
			count_word++;
		while (s[j] && s[j] != c)
			j++;
	}
	return (count_word);
}

static char	*copy_word(char const *s, unsigned int start, size_t len)
{
	char			*str;
	unsigned int	k;

	k = 0;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (k < len)
	{
		str[k] = s[start + k];
		k++;
	}
	str[k] = '\0';
	return (str);
}

char	**ft_split(char const *s, char c)
{
	unsigned int	j;
	unsigned int	i;
	unsigned int	len_word;
	char			**res;

	i = 0;
	j = 0;
	len_word = word_count(s, c, ft_strlen(s));
	res = (char **)malloc((len_word + 1) * sizeof(char *));
	if (!s || !res)
		return (0);
	while (i < word_count(s, c, ft_strlen(s)))
	{
		len_word = 0;
		while (s[j] && s[j] == c)
			j++;
		while (s[j + len_word] && s[j + len_word] != c)
			len_word++;
		res[i] = copy_word(s, j, len_word);
		i++;
		j = j + len_word;
	}
	res[i] = 0;
	return (res);
}
