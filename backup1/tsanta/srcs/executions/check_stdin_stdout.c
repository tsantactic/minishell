#include "../../include/minishell.h"

int add_stdin(t_token **token, int len)
{
    int i = 0;
    int set = 0;
    int is_redir_in = 0;
    while ( i < len)
    {
        if ((token[i]->type == CMD || token[i]->type == REDIR_OUT) && !is_redir_in)
        {
            set = 1;
        }
        if (token[i]->type == REDIR_IN || token[i]->type == REDIR_HEREDOC)
        {
            is_redir_in = 1;
            set = 0;
        }
        i++;
    }
    return (set);
}
void check_stdin(t_cmd *cmd)
{
    int num_pipes = cmd->nb_pipe;
    int i = 0;
    while (i < num_pipes)
	{   
        cmd->commands_arg[i][0]->stdin_set = 0;
        if (add_stdin(cmd->commands_arg[i], cmd->commands_arg[i][0]->total_len) == 1)
            cmd->commands_arg[i][0]->stdin_set = 1;
        i++;
    }
    if (add_stdin(cmd->commands_arg[num_pipes], cmd->commands_arg[num_pipes][0]->total_len))
        cmd->commands_arg[num_pipes][0]->stdin_set = 1;
    else
        cmd->commands_arg[num_pipes][0]->stdin_set = 0;
}
int add_stdout(t_token **token, int len)
{
    int i = 0;
    int is_redir_out = 0;
    int is_cmd = 0;
    while ( i < len)
    {
        if (token[i]->type == CMD)
        {
            is_cmd = 1;        
        }
        if (token[i]->type == REDIR_OUT)
        {
            is_redir_out = 1;
        }
        i++;
    }
    if (is_redir_out == 1 && is_cmd == 1)
    {
        return(1);
    }
    return (0);
}
void check_stdout(t_cmd *cmd)
{
    int num_pipes = cmd->nb_pipe;
    int i = 0;
    while (i < num_pipes)
	{
        cmd->commands_arg[i][0]->stdout_set = 0;
        if (add_stdout(cmd->commands_arg[i], cmd->commands_arg[i][0]->total_len) == 1)
            cmd->commands_arg[i][0]->stdout_set = 1;
        i++;
    }
    if (add_stdout(cmd->commands_arg[num_pipes], cmd->commands_arg[num_pipes][0]->total_len) == 1)
        cmd->commands_arg[num_pipes][0]->stdout_set = 1;
    else
        cmd->commands_arg[num_pipes][0]->stdout_set = 0;
}