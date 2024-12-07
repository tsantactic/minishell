
bug
 STATUS DE RETOUR: 
 Quelques codes de retour spécifiques :
1 : Erreur générique.
2 : Mauvaise utilisation de la commande.
126 : Commande trouvée mais non exécutable (souvent une erreur de permission).
127 : Commande introuvable.

OLDPWD
export sur alphanum
echo $"HOME" = HOME
# Guide pour l'Implémentation d'un Minishell

## Compétences à Maîtriser

1. **Programmation en C**
   - Gestion des chaînes de caractères
   - Pointeurs et manipulation de mémoire
   - Structures de données et gestion de la mémoire

2. **Connaissance des Systèmes Unix/Linux**
   - Commandes shell et leur fonctionnement
   - Gestion des processus et des signaux
   - Gestion des fichiers et des permissions

3. **Gestion des Processus**
   - Appels système : `fork()`, `exec()`, `wait()`, `signal()`
   - Création et gestion des processus enfants
   - Communication entre processus

4. **Manipulation des Fichiers**
   - Appels système pour les fichiers : `open()`, `read()`, `write()`, `close()`
   - Gestion des descripteurs de fichiers

5. **Analyse de Commandes**
   - Parsing des lignes de commande
   - Séparation des arguments et gestion des délimiteurs

6. **Gestion des Signaux**
   - Configuration et gestion des signaux avec `signal()` ou `sigaction()`
   - Réaction aux interruptions et autres signaux

## 1. Conception Initiale

La conception initiale est essentielle pour poser les bases de ton minishell. Cela implique de définir les fonctionnalités et de planifier l'architecture du code.

### Définition des Fonctionnalités de Base

1. **Exécution des Commandes**
   - **Lecture des Commandes :** Lire les commandes de l'utilisateur à partir de l'entrée standard.
   - **Analyse des Commandes :** Extraire les arguments et options des commandes.
   - **Exécution des Programmes :** Utiliser `fork()` et `exec()` pour exécuter les programmes dans des processus séparés.

2. **Gestion des Pipes (`|`)**
   - **Connexion des Flux :** Connecter la sortie d'une commande à l'entrée d'une autre en utilisant des pipes.
   - **Création de Processus :** Gérer la création des processus nécessaires pour chaque partie du pipeline.

3. **Gestion des Redirections (`>`, `<`)**
   - **Redirection de Sortie (`>`):** Diriger la sortie d'une commande vers un fichier.
   - **Redirection d'Entrée (`<`):** Diriger l'entrée d'une commande depuis un fichier.

4. **Gestion des Background Processes (`&`)**
   - **Exécution en Arrière-Plan :** Permettre l'exécution des commandes en arrière-plan pour que le shell continue à recevoir des commandes.

### Planification de l'Architecture du Code

1. **Modules de Traitement des Commandes**
   - **Lecture et Séparation :** Fonction pour lire et séparer les tokens (arguments) des lignes de commande.
   - **Analyse des Redirections et Pipes :** Fonctions pour identifier et gérer les redirections et les pipes.

2. **Modules de Gestion des Processus**
   - **Création de Processus :** Utiliser `fork()` pour créer des processus enfants.
   - **Exécution des Commandes :** Utiliser `execvp()` pour remplacer le processus enfant par la commande exécutée.
   - **Gestion des Terminaisons :** Utiliser `wait()` pour récupérer les codes de sortie des processus enfants.

3. **Modules de Gestion des Entrées/Sorties**
   - **Redirection des Flux :** Gérer les fichiers et les redirections avec `open()`, `read()`, `write()`, et `close()`.

### Exemples de Fonctionnalités à Planifier

- **Prompt :** Afficher et mettre à jour le prompt pour indiquer l'état du shell.
- **Gestion des Erreurs :** Traiter les erreurs liées aux commandes ou aux appels système.

## 2. Création d'une Structure de Base

Après la conception, passe à la mise en place de la structure de base du minishell.

### Écriture de la Boucle Principale

- **Affichage du Prompt :** Créer une boucle principale qui affiche le prompt à l'utilisateur.
- **Lecture de l'Entrée :** Lire la ligne de commande entrée par l'utilisateur avec des fonctions comme `getline()`.
- **Traitement de la Commande :** Passer la ligne de commande à la fonction d'analyse.

### Gestion de l'Entrée Utilisateur

- **Lecture de Ligne :** Utiliser `fgets()` ou `getline()` pour lire l'entrée.
- **Gestion des Erreurs :** Traiter les erreurs potentielles lors de la lecture.

## 3. Analyse des Commandes

Développe les fonctions pour analyser et traiter les commandes.

### Parsing

- **Séparation en Tokens :** Fonction pour séparer la ligne de commande en tokens basés sur les espaces et les délimiteurs.
- **Identification des Arguments :** Extraire et identifier les arguments et les options.

### Gestion des Espaces et des Délimiteurs

- **Traitement des Espaces :** Gérer les espaces pour séparer les arguments.
- **Gestion des Délimiteurs :** Traiter les pipes et les redirections pour une analyse correcte.

## 4. Exécution des Commandes

Implémente la logique pour exécuter les commandes.

### Création de Processus

- **Utilisation de `fork()` :** Créer des processus enfants pour chaque commande.
- **Gestion des Processus :** Assurer la gestion des processus enfants et éviter les fuites de mémoire.

### Exécution de la Commande

- **Utilisation de `execvp()` :** Remplacer le processus enfant par la commande exécutée.
- **Gestion des Terminaisons :** Assurer que le processus enfant termine correctement et que les ressources sont libérées.

### Attente de la Fin des Processus

- **Utilisation de `wait()` :** Attendre la fin des processus enfants pour récupérer les codes de sortie.

## 5. Gestion des Pipes et des Redirections

Implémente la gestion des pipes et des redirections pour permettre la communication entre les commandes.

### Pipes

- **Création et Gestion :** Créer des pipes avec `pipe()` pour connecter les sorties et entrées des commandes.
- **Redirection des Flux :** Configurer les descripteurs de fichiers pour les pipes.

### Redirections

- **Gestion des Fichiers :** Utiliser `open()`, `read()`, `write()`, et `close()` pour les redirections d'entrée et de sortie.

## 6. Gestion des Signaux

Configure la gestion des signaux pour gérer les interruptions et autres signaux.

### Signal Handling

- **Configuration des Gestionnaires :** Utiliser `signal()` ou `sigaction()` pour configurer les gestionnaires de signaux comme `SIGINT` et `SIGTSTP`.
- **Gestion des Interruptions :** Réagir correctement aux signaux pour interrompre ou mettre en pause les processus.

## 7. Tests et Débogage

Teste et débogue ton minishell pour garantir qu'il fonctionne correctement.

### Test des Fonctionnalités

- **Tests Unitaires :** Tester chaque fonctionnalité individuellement pour vérifier son bon fonctionnement.
- **Scénarios Combinés :** Tester des scénarios combinant plusieurs fonctionnalités pour vérifier l'intégration.

### Débogage

- **Outils de Débogage :** Utiliser `gdb` ou d'autres outils pour identifier et résoudre les problèmes.
- **Analyse des Erreurs :** Examiner les erreurs et les comportements inattendus pour corriger les bogues.

## 8. Optimisation et Améliorations

Après la mise en œuvre des fonctionnalités de base, optimise et améliore ton minishell.

### Optimisation

- **Amélioration des Performances :** Optimiser le code pour améliorer les performances.
- **Gestion de la Mémoire :** Optimiser la gestion de la mémoire pour éviter les fuites et les problèmes de performance.

### Extensions

- **Fonctionnalités Supplémentaires :** Ajouter des fonctionnalités comme la gestion des variables d'environnement, l’historique des commandes, et le support des scripts.
- **Interface Utilisateur :** Améliorer l'interface utilisateur pour une meilleure expérience, comme le support des couleurs et l'autocomplétion.


# Fonctionnalités d'un Projet Minishell

## 1. Exécution des Commandes
- **Exécution de Commandes Simples :** Exécuter des commandes comme `ls`, `echo`, ou `cat`. Utilisation de `fork` et `execve` pour créer des processus enfants et remplacer leur image par la commande.

## 2. Gestion des Arguments
- **Arguments des Commandes :** Traitement des arguments passés aux commandes. Par exemple, pour `ls -l`, `-l` est un argument qui doit être transmis correctement à la commande.

## 3. Redirections
- **Redirection de l’Entrée (`<`) :** Rediriger l'entrée d'une commande depuis un fichier. Exemple : `sort < file.txt`.
- **Redirection de la Sortie (`>`) :** Rediriger la sortie d'une commande vers un fichier. Exemple : `echo "Hello" > file.txt`.
- **Redirection de la Sortie d'Erreur (`2>`) :** Rediriger les erreurs vers un fichier. Exemple : `command 2> error.txt`.

## 4. Pipes
- **Communication entre Processus :** Utiliser des pipes pour passer la sortie d'une commande comme entrée à une autre. Exemple : `ls | grep "file"`.

## 5. Gestion des Signaux
- **Interruption (`SIGINT`) :** Gérer les interruptions comme `Ctrl+C` pour arrêter les commandes en cours.
- **Changement d’État des Processus Enfants (`SIGCHLD`) :** Gérer la fin des processus enfants pour éviter les processus zombies.

## 6. Historique des Commandes
- **Historique :** Conserver et rappeler les commandes précédemment exécutées, similaire à `history` dans Bash.

## 7. Gestion des Variables d’Environnement
- **Affichage des Variables :** Afficher les variables d’environnement avec `printenv` ou `env`.
- **Définition des Variables :** Définir ou modifier les variables d’environnement avec `export`. Exemple : `export VAR=value`.
- **Suppression des Variables :** Supprimer des variables d’environnement avec `unset`. Exemple : `unset VAR`.

## 8. Gestion des Chemins
- **Recherche de Commandes :** Utiliser la variable `PATH` pour rechercher et exécuter des commandes sans avoir à spécifier leur chemin complet.

## 9. Gestion des Erreurs
- **Messages d’Erreur :** Fournir des messages d’erreur clairs et utiles en cas d'échec de commande ou d'arguments invalides.

## 10. Support des Commandes en Arrière-Plan
- **Exécution en Arrière-Plan (`&`) :** Permettre l'exécution des commandes en arrière-plan pour ne pas bloquer l’invite de commande. Exemple : `sleep 10 &`.

## 11. Auto-complétion
- **Complétion de Commandes :** Fournir une fonctionnalité de complétion automatique pour les commandes et les arguments.

## 12. Substitution de Commandes
- **Command Substitution (`$(...)` ou `` `...` ``) :** Utiliser les résultats d’une commande comme arguments pour une autre commande.

## 13. Gestion des Aliases
- **Définition des Aliases :** Créer des alias pour les commandes couramment utilisées pour simplifier leur utilisation.

## 14. Gestion des Directives de Shell
- **Commandes Internes :** Implémenter des commandes internes comme `cd` pour changer le répertoire de travail actuel, `exit` pour quitter le minishell, et `pwd` pour afficher le répertoire courant.


# Architecture du Projet Minishell

## Structure des Dossiers et des Fichiers

Organisez votre projet avec la structure suivante pour une meilleure gestion et modularité du code :

minishell/
├── src/
│ ├── main.c
│ ├── shell.c
│ ├── command.c
│ ├── process.c
│ ├── io.c
│ ├── signal.c
│ └── utils.c
├── include/
│ ├── shell.h
│ ├── command.h
│ ├── process.h
│ ├── io.h
│ ├── signal.h
│ └── utils.h
├── Makefile
└── README.md



## Description des Modules

### 1. `main.c`
- **Responsabilité :** Point d'entrée du programme.
- **Contenu :** Contient la fonction `main` qui initialise le minishell et appelle la boucle principale.
- **Fonctions clés :**
  - `int main(int argc, char **argv)`: Point d'entrée, initialisation, et boucle de lecture des commandes.

### 2. `shell.c` et `shell.h`
- **Responsabilité :** Gestion de la boucle principale du shell.
- **Contenu :** 
  - La boucle principale qui lit les entrées utilisateur.
  - Gestion des signaux.
  - Fonction de traitement des commandes.
- **Fonctions clés :**
  - `void run_shell()`: Boucle principale qui lit les entrées utilisateur et appelle les fonctions appropriées.
  - `void handle_signals()`: Configuration des gestionnaires de signaux.

### 3. `command.c` et `command.h`
- **Responsabilité :** Analyse et traitement des commandes.
- **Contenu :** 
  - Fonctionnalités pour analyser les entrées utilisateur et extraire les commandes et leurs arguments.
  - Gestion des commandes internes (`cd`, `exit`, etc.).
- **Fonctions clés :**
  - `char **parse_command(char *input)`: Analyse de l'entrée pour extraire les tokens (commande et arguments).
  - `int execute_command(char **args)`: Exécution des commandes, y compris les commandes internes.

### 4. `process.c` et `process.h`
- **Responsabilité :** Gestion des processus.
- **Contenu :** 
  - Création de processus avec `fork`.
  - Exécution de commandes avec `execve`.
  - Attente des processus avec `waitpid`.
- **Fonctions clés :**
  - `void fork_and_execute(char **args)`: Création de processus et exécution de la commande spécifiée.
  - `void wait_for_child()`: Attente de la fin des processus enfants.

### 5. `io.c` et `io.h`
- **Responsabilité :** Gestion des entrées/sorties, redirections, et pipes.
- **Contenu :** 
  - Fonctionnalités pour rediriger les entrées et sorties des commandes.
  - Gestion des pipes pour les commandes en chaîne.
- **Fonctions clés :**
  - `void handle_redirections(char **args)`: Gestion des redirections (`<`, `>`, `2>`).
  - `void setup_pipes(char **args)`: Configuration des pipes pour les commandes enchaînées.

### 6. `signal.c` et `signal.h`
- **Responsabilité :** Gestion des signaux.
- **Contenu :** 
  - Configuration des gestionnaires de signaux pour les interruptions et autres signaux pertinents.
- **Fonctions clés :**
  - `void setup_signal_handlers()`: Configuration des gestionnaires pour les signaux comme `SIGINT` et `SIGCHLD`.

### 7. `utils.c` et `utils.h`
- **Responsabilité :** Fonctions utilitaires générales.
- **Contenu :** 
  - Fonctions auxiliaires qui ne rentrent pas directement dans les autres modules, comme la gestion de la mémoire ou le traitement des chaînes.
- **Fonctions clés :**
  - `char **str_split(char *str, char delim)`: Fonction pour diviser une chaîne en tokens.
  - `void free_split(char **array)`: Libération de mémoire pour les tableaux de chaînes.

## Interconnexion des Modules

- **`main.c`** appelle `run_shell()` dans `shell.c` pour démarrer la boucle principale du shell.
- **`shell.c`** utilise `parse_command()` de `command.c` pour analyser les entrées utilisateur.
- **`command.c`** appelle `fork_and_execute()` dans `process.c` pour créer des processus et exécuter des commandes.
- **`command.c`** interagit avec `io.c` pour gérer les redirections et les pipes.
- **`signal.c`** configure les gestionnaires de signaux qui sont utilisés dans `shell.c`.
- **`utils.c`** fournit des fonctions utilitaires qui peuvent être appelées depuis n'importe quel module.

## Gestion des Erreurs et Tests

- **Gestion des Erreurs :** Implémentez des mécanismes robustes pour gérer les erreurs dans chaque module. Assurez-vous de retourner des codes d'erreur appropriés et de fournir des messages clairs.
- **Tests :** Écrivez des tests pour chaque fonctionnalité individuelle. Utilisez des tests unitaires pour vérifier le bon fonctionnement des fonctions et des tests d'intégration pour vérifier l'interaction entre les modules.

## Documentation

- **README.md :** Fournit une vue d'ensemble du projet, y compris l'architecture, les fonctionnalités, et les instructions pour compiler et exécuter le minishell.
- **Commentaires dans le Code :** Commentez le code pour expliquer les parties complexes et les choix de conception afin de faciliter la maintenance et la compréhension.

Cette structure et cette organisation vous aideront à développer un minishell modulaire et maintenable. Bonne chance avec votre projet !
