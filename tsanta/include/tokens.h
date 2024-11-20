/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:35:49 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/10 17:19:28 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

typedef struct s_cmd t_cmd;

void type_token(t_cmd *cmd);
void	free_tokens(t_cmd *cmd);
void 	free_token_list(t_cmd *cmd);

#endif