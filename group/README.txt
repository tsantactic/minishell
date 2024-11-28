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

simple command:
builtins:ko
redirection ok
command simple: ok
syntax error: ok
signal: ko
exitstatus: ko
error file descriptor: ok
...
command avec pipe:
builtins:ko
redirection ko
command simple: ko
syntax error: ok
signal: ko
exitstatus: ko
error file descriptor: ok

/****************************************************/
Voici une description des utilités des fonctions externes listées pour un projet **Minishell** conforme aux attentes de l'école 42 :

---

### **Fonctions liées à l'entrée et la ligne de commande :**
1. **`readline`** : Récupère une ligne de commande saisie par l'utilisateur avec édition en ligne (inclut la gestion de l'historique).
2. **`rl_clear_history`** : Efface l'historique des commandes stockées par `readline`.
3. **`rl_on_new_line`** : Indique que l'utilisateur commence une nouvelle ligne (utile pour actualiser l'affichage après un signal).
4. **`rl_replace_line`** : Remplace la ligne courante dans `readline`.
5. **`rl_redisplay`** : Réaffiche la ligne en cours d'édition.
6. **`add_history`** : Ajoute la commande actuelle dans l'historique de `readline`.

---

### **Entrées/Sorties générales :**
7. **`printf`** : Affiche du texte formaté sur la sortie standard.
8. **`write`** : Écrit des données dans un fichier ou un descripteur de fichier.
9. **`read`** : Lit des données à partir d'un descripteur de fichier.
10. **`isatty`** : Vérifie si un descripteur correspond à un terminal interactif.
11. **`ttyname`** : Donne le chemin du fichier de terminal associé à un descripteur.
12. **`ttyslot`** : Trouve l'entrée associée au terminal dans la base des terminaux.
13. **`ioctl`** : Permet des commandes spécifiques aux périphériques (souvent pour manipuler les terminaux).

---

### **Gestion de mémoire et erreurs :**
14. **`malloc`** : Alloue de la mémoire dynamique.
15. **`free`** : Libère la mémoire allouée par `malloc`.
16. **`strerror`** : Retourne une chaîne décrivant un code d'erreur.
17. **`perror`** : Affiche un message d'erreur sur la sortie d'erreur standard.

---

### **Fichiers et répertoires :**
18. **`access`** : Vérifie les droits d'accès à un fichier (lecture, écriture, exécution).
19. **`open`** : Ouvre un fichier et retourne un descripteur.
20. **`close`** : Ferme un descripteur de fichier.
21. **`unlink`** : Supprime un fichier ou un lien symbolique.
22. **`stat`**, **`lstat`**, **`fstat`** : Récupèrent des informations sur un fichier.
23. **`opendir`** : Ouvre un répertoire.
24. **`readdir`** : Lit les entrées dans un répertoire ouvert.
25. **`closedir`** : Ferme un répertoire ouvert.

---

### **Exécution de processus :**
26. **`fork`** : Crée un processus enfant en dupliquant le processus parent.
27. **`wait`**, **`waitpid`**, **`wait3`**, **`wait4`** : Attendent la terminaison d'un ou plusieurs processus enfants.
28. **`execve`** : Remplace l'image du processus courant par celle d'un programme à exécuter.
29. **`exit`** : Termine un processus avec un code de retour.

---

### **Redirections et pipes :**
30. **`dup`**, **`dup2`** : Dupliquent un descripteur de fichier pour rediriger les entrées/sorties.
31. **`pipe`** : Crée un canal de communication unidirectionnel entre deux processus.

---

### **Gestion des signaux :**
32. **`signal`** : Définit un gestionnaire de signal.
33. **`sigaction`** : Configure un gestionnaire de signal avancé.
34. **`sigemptyset`**, **`sigaddset`** : Manipulent les ensembles de signaux.
35. **`kill`** : Envoie un signal à un processus ou un groupe de processus.

---

### **Fonctions utilitaires :**
36. **`getcwd`** : Récupère le chemin absolu du répertoire de travail actuel.
37. **`chdir`** : Change le répertoire courant.
38. **`getenv`** : Récupère la valeur d'une variable d'environnement.

---

### **Fonctions liées au terminal :**
39. **`tcsetattr`**, **`tcgetattr`** : Configurent ou lisent les paramètres du terminal.
40. **`tgetent`** : Charge les capacités du terminal depuis la base de données termcap.
41. **`tgetflag`**, **`tgetnum`**, **`tgetstr`** : Lisent les capacités du terminal (booléennes, numériques ou chaînes).
42. **`tgoto`** : Calcule une séquence d'échappement de déplacement du curseur.
43. **`tputs`** : Affiche une séquence de contrôle du terminal.

---

Ces fonctions couvrent les bases pour implémenter un shell Unix minimal : gestion des entrées, exécution de commandes, redirections, gestion de processus et interaction avec le terminal.





