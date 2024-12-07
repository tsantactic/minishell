/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   annexe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:54:16 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/26 14:37:55 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/minishell.h"

// int handle_backslash_sequence(const char *str, size_t *index, char *result, size_t *res_index, t_cmd *cmd)
// {
//     size_t backslash_count = 0;

// 	(void)cmd;
//     // Compte les backslashes consécutifs
//     while (str[*index] == '\\')
//     {
//         backslash_count++;
//         (*index)++;
//     }
// 	printf("bask %ld", backslash_count);
//     // Si le nombre de backslashes est pair, ajoute la moitié d'entre eux
//     for (size_t i = 0; i < backslash_count / 2; i++)
//     {
//         result[(*res_index)++] = '\\';
//     }

//     // Si le nombre de backslashes est impair et suivi de $, ajoute un littéral $
//     if (backslash_count % 2 == 1)
//     {
//         if (str[*index] == '$')
//         {
//             result[(*res_index)++] = '$';
//             (*index)++; // Ignore le $
//         }
//         else
//         {
//             result[(*res_index)++] = '\\'; // Ajoute le backslash restant
//         }
//     }

//     return 1; // Indique qu'un traitement a été effectué
// }


// trier environ pour export

// void display_export(char **envp)
// {
//     // Comptez les variables d'environnement
//     int count = 0;
//     while (envp[count])
//         count++;

//     // Créez une copie de l'environnement
//     char **sorted_env = malloc(sizeof(char *) * (count + 1));
//     if (!sorted_env)
//     {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }
//     for (int i = 0; i < count; i++)
//         sorted_env[i] = envp[i];
//     sorted_env[count] = NULL;

//     // Tri par sélection
//     for (int i = 0; i < count - 1; i++)
//     {
//         int min_idx = i;
//         for (int j = i + 1; j < count; j++)
//         {
//             if (strcmp(sorted_env[j], sorted_env[min_idx]) < 0)
//                 min_idx = j;
//         }
//         // Échangez les éléments si nécessaire
//         if (min_idx != i)
//         {
//             char *temp = sorted_env[i];
//             sorted_env[i] = sorted_env[min_idx];
//             sorted_env[min_idx] = temp;
//         }
//     }

//     // Affichez chaque variable avec "declare -x"
//     for (int i = 0; i < count; i++)
//     {
//         char *equals = strchr(sorted_env[i], '=');
//         if (equals)
//         {
//             *equals = '\0';
//             printf("declare -x %s=\"%s\"\n", sorted_env[i], equals + 1);
//             *equals = '='; // Rétablir la chaîne d'origine
//         }
//         else
//         {
//             printf("declare -x %s\n", sorted_env[i]);
//         }
//     }

//     free(sorted_env);
// }

// COPY ENVIRONNEMENT
// char	**copy_env(char **envp)
// {
// 	int	count;
// 	int	i;
// 	char	**env_copy;

// 	count = 0;
// 	i = 0;
// 	env_copy = malloc(sizeof(char *) * (count + 1));
// 	if (!env_copy)
// 	{
// 		free(env_copy);
// 		return (NULL);
// 	}
// 	while (i < count)
// 	{
// 		env_copy[i] = ft_strdup(envp[i]);
// 		if (!env_copy[i])
// 		{
// 			perror("strdup");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	env_copy = NULL;
// 	return (env_copy);	
// }
// void	free_env(char **envp)
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i])
// 		free(envp[i]);
// 	free(envp);
// }