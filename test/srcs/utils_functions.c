/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:44:17 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/23 10:42:35 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i])
			free(str[i]);
		i++;
	}
	free(str);
}

void	ft_free_dp(char **str, int len)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (i < len)
	{
		if (str[i])
			free(str[i]);
		i++;
	}
	free(str);
}

char	*ft_strndup(const char *s, int n)
{
	char	*dup;
	int		i;

	dup = (char *)malloc((n + 1) * sizeof(char));
	if (!dup)
	{
		return (NULL);
	}
	i = 0;
	while (i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[n] = '\0';
	return (dup);
}

char	*ft_strjoin_copy(char const *s1, char const *s2)
{
	char	*new_s;
	int		i;
	int		j;

	new_s = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
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

void	free_token_list(t_cmd *cmd)
{
	int	j;

	j = 0;
	while (j < cmd->len_arg)
	{
		free(cmd->token_arg[j]);
		j++;
	}
	free(cmd->token_arg);
	cmd->token_arg = NULL;
	free_tokens(cmd);
}
