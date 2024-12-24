#include "../include/minishell.h"

int	check_syntax_one(t_token **tokens, int len_tokens, int i)
{
	if ((tokens[i]->type == REDIR_IN || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC) && i + 1 == len_tokens)
	{
		set_st(2);
		return (-2);
	}
	return (0);
}
