#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    char **args; // tableau de chaînes de caractères
} Command;

int contains_here_doc(Command *cmd, int i) {
    // Vérifie si cmd->args[i] contient "<<"
    char *ptr = strstr(cmd->args[i], "<<");
    if (ptr != NULL) {
        // Vérifie qu'il y a des caractères avant et après "<<"
        if (ptr != cmd->args[i] && *(ptr + 2) != '\0' && !isspace(*(ptr + 2))) {
            return 1; // True, il y a un here-doc valide
        }
    }
    return 0; // False, pas de here-doc valide
}

void tokenize_here_doc(Command *cmd, int i) {
    char *ptr = strstr(cmd->args[i], "<<");
    if (ptr != NULL) {
        // Terminer la chaîne à "<<"
        *ptr = '\0';
        ptr += 2; // Avancer après "<<"
        
        // Ignorer les espaces
        while (isspace(*ptr)) ptr++;

        // Extraire le premier mot après "<<"
        char *second_word = strtok(ptr, " ");
        if (second_word != NULL) {
            // Afficher les mots extraits
            printf("Avant <<: %s\n", cmd->args[i]);
            printf("Après <<: %s\n", second_word);
        }
    }
}

int main() {
    // Exemple d'utilisation
    Command cmd;
    char *arguments[] = {"ls", ">>output", "ls<<l", "echo <<", "cat << file", NULL};
    cmd.args = arguments;

    for (int i = 0; cmd.args[i] != NULL; i++) {
        if (contains_here_doc(&cmd, i)) {
            printf("Il y a un here-doc valide dans: %s\n", cmd.args[i]);
            tokenize_here_doc(&cmd, i);
        }
    }
    
    return 0;
}
