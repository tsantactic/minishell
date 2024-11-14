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