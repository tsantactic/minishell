voici les etapes deja fais:
-creation readline () (minishell.c)
-recuperation des contenues de readline (parsing.c dans pargse_cmd())
-utilisation de count_arg() pout compter ce que on dois avoir pour les arguments de readline / total des argument possible. On a gerer aussi les qoutes non fermer(parsing_utils.c )
-utilisation de stock_arg() pour separer avec les espace les arguments (stock_arg.c)
-utilisation de stock_token() pour recuperer les argument meme si ils sont dans les quotes fermer , les entre redirection comme ls<ls ou autre (lexing.c)
-utilisation de type_token() pour typer les arguments retourner par stock_token() par les enum dans le header (void le minishell.h) (type_arg.c)
-utilisation de move_quote() apres avoir connaitre leur type si l'argument est entre quote fermer (parsing.c)
-utilisation de check_error_arg() pour gerer les syntax error
-utilisation de count_pipe() pour compter les pipes possible dans l'argument (parsing.c)
-utilisation de fork_exec() si il n y a pas de pipe (juste commande simple pas de pipe) et il y a fonction pour voir si la commande premiere est builtins (pipe_normal.c) (execution_comand.c)
-utilisation de execute_with_pipes() si il y a des pipes (TODO) (execution_comand.c)
-chemin /bin/ et /usr/bin/ + command

 /*pas encore fini*/
 builtins, 
 les redirections (pour sans pipe ok, aucun pipe non)

 pipe et mutltipipe, 
 signal 
 is directory / no such file directory
 exit_status ($?)
 autres leaks,
 reduire les lignes des fonctions,


exit_status:
sandriam@c1r7s14:~/Documents/42/work/minishell/tsanta_minishell$ /bfr
bash: /bfr: No such file or directory
sandriam@c1r7s14:~/Documents/42/work/minishell/tsanta_minishell$ $?
127: command not found

sandriam@c1r7s14:~/Documents/42/work/minishell/tsanta_minishell$ /bin/ls/l
bash: /bin/ls/l: Not a directory
sandriam@c1r7s14:~/Documents/42/work/minishell/tsanta_minishell$ $?
126: command not found

sandriam@c1r7s14:~/Documents/42/work/minishell/tsanta_minishell$ /bin/ls ou ls
Chapter5-WritingYourOwnShell.pdf  Libft      minishell.pdf    README.txt  subject_guide.pdf
e				  Makefile   readline_1.supp  run.txt	  test_heredoc.c
include				  minishell  readline.supp    srcs
sandriam@c1r7s14:~/Documents/42/work/minishell/tsanta_minishell$ $?
0: command not found

sandriam@c1r7s14:~/Documents/42/work/minishell/tsanta_minishell$ /bin/
bash: /bin/: Is a directory
sandriam@c1r7s14:~/Documents/42/work/minishell/tsanta_minishell$ $?
126: command not found


test syntax error heredoc que heredoc doit executer:
sandriam@c1r4s10:~/Documents/42/work/minishell/tsanta_minishell$ cat << o << e < <
bash: syntax error near unexpected token `<'
> o
> e
sandriam@c1r4s10:~/Documents/42/work/minishell/tsanta_minishell$ cat << o << e <
> o
> e
bash: syntax error near unexpected token `newline'
sandriam@c1r4s10:~/Documents/42/work/minishell/tsanta_minishell$ cat | ls -a | ls << e < <
bash: syntax error near unexpected token `<'
> e
sandriam@c1r4s10:~/Documents/42/work/minishell/tsanta_minishell$ cat | ls -a | ls << e < >
bash: syntax error near unexpected token `>'
> ^C
sandriam@c1r4s10:~/Documents/42/work/minishell/tsanta_minishell$ cat << eof <
> 
bash: warning: here-document at line 10 delimited by end-of-file (wanted `eof')
bash: syntax error near unexpected token `newline'
sandriam@c1r4s10:~/Documents/42/work/minishell/tsanta_minishell$ cat << eof < <
bash: syntax error near unexpected token `<'
> 
bash: warning: here-document at line 11 delimited by end-of-file (wanted `eof')
cat Makefile | grep -i name |

void redir_exec(t_cmd *cmd, char **envp)
{
    int count_heredoc = 0;
    int **pipe_heredoc;
    // Comptage des heredocs
    for (int i = 0; i < cmd->len_tokens; i++)
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
            count_heredoc++;

    // Allocation des pipes heredocs
    pipe_heredoc = malloc(sizeof(int *) * count_heredoc);
    for (int i = 0; i < count_heredoc; i++)
        pipe_heredoc[i] = malloc(sizeof(int) * 2);

    int heredoc_index = 0;
    for (int i = 0; i < cmd->len_tokens; i++)
    {
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
        {
            char *delimiter = cmd->tokens[i + 1]->value;
            pipe(pipe_heredoc[heredoc_index]);
            loop_heredoc(delimiter, pipe_heredoc[heredoc_index]);
            heredoc_index++;
        }
    }

    // Connecter le dernier heredoc à stdin
    if (count_heredoc > 0)
    {
        dup2(pipe_heredoc[count_heredoc - 1][0], STDIN_FILENO);
        close(pipe_heredoc[count_heredoc - 1][0]);
        close(pipe_heredoc[count_heredoc - 1][1]);
    }

    // Gestion des commandes et exécution
    char *command = copy_command_arg(cmd, NULL, NULL);
    if (command)
    {
        char *path = ft_find_path(command, envp);
        ft_execute_command(cmd, path, NULL, envp);
        free(path);
    }

    // Libération
    for (int i = 0; i < count_heredoc; i++)
        free(pipe_heredoc[i]);
    free(pipe_heredoc);
}
void handle_heredocs(t_cmd *cmd)
{
    int count_heredoc = 0;
    int **pipe_heredoc;

    // Compter le nombre de heredocs
    for (int i = 0; i < cmd->len_tokens; i++)
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
            count_heredoc++;

    // Allouer les pipes pour chaque heredoc
    pipe_heredoc = malloc(sizeof(int *) * count_heredoc);
    for (int i = 0; i < count_heredoc; i++)
        pipe_heredoc[i] = malloc(sizeof(int) * 2);

    // Traiter chaque heredoc
    int heredoc_index = 0;
    for (int i = 0; i < cmd->len_tokens; i++)
    {
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
        {
            char *delimiter = cmd->tokens[i + 1]->value;
            pipe(pipe_heredoc[heredoc_index]); // Créer un pipe pour le heredoc
            loop_heredoc(delimiter, pipe_heredoc[heredoc_index]); // Remplir le pipe
            heredoc_index++;
        }
    }

    // Créer un pipe final pour fusionner les contenus
    int final_pipe[2];
    pipe(final_pipe);

    // Fusionner tous les pipes heredoc dans le pipe final
    for (int i = 0; i < count_heredoc; i++)
    {
        char buffer[1024];
        ssize_t bytes_read;

        close(pipe_heredoc[i][1]); // Fermer l'écriture dans le pipe heredoc
        while ((bytes_read = read(pipe_heredoc[i][0], buffer, sizeof(buffer))) > 0)
            write(final_pipe[1], buffer, bytes_read); // Copier dans le pipe final
        close(pipe_heredoc[i][0]); // Fermer la lecture après fusion
        free(pipe_heredoc[i]);
    }

    free(pipe_heredoc);

    // Fermer l'écriture dans le pipe final et connecter à STDIN
    close(final_pipe[1]);
    dup2(final_pipe[0], STDIN_FILENO);
    close(final_pipe[0]);
}

