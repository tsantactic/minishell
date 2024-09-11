#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	ft_strncmp(char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*sone;
	unsigned char	*stwo;

	sone = (unsigned char *)s1;
	stwo = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (sone[i] == stwo[i] && (i < n - 1) && sone[i])
		i++;
	return (sone[i] - stwo[i]);
}

void ft_check_arg(char *read_line, char **envp)
{

}

void find_path()
{

}

void find_path_name()
{

}

void execute_cmd()
{

}

int main(int arg, char **argv, char **envp)
{
	char *read_line;
	(void) arg;
	(void) argv;	
	while (1)
	{
		read_line = readline("Minishell > ");
		if (ft_strncmp(read_line, "exit", 5) == 0)
			break;
		ft_check_arg(read_line, envp);
	}
	free(read_line);
	return (0);
}
