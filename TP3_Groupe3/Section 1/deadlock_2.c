#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// Question 1:
// 1. Exclusion mutuelle: L'exclusion mutuelle est représentée aux lignes 
// 2. Detention et attente: 
// 3. Pas de requisition:
// 4. Attente circulaire:

// Question 2: Ce code fait référence au problème des lecteurs/rédacteurs

// Question 3: Le déplacement des deux lignes après le while à la position 24 et 25 permet d'écrire dans le fichier avant de lire.
// En effet, avant l'écriture se faisait après que les processus enfants soit terminés (wait ligne 41). Sauf que la lecture se fait
// dans un processus enfant à la ligne 30. Donc, cela créer une attente circulaire. En déplaçant ces lignes, ça enlève l'attente 
// circulaire et l'enfant n'attend plus après le parent pour qu'il écrive dans le fichier.

int main() {
    int fd[2];

    pipe(fd);

    char string[6] = "Salut!";             // déplacement
    write(fd[1], &string, strlen(string));

    if(!fork()){
        close(fd[1]);
        char* string[6];    
        read(fd[0], string, 6);
        exit(0);
    }

    if(!fork()){
        close(fd[1]);
        close(fd[0]);
        execlp("ls", "ls", "-l", NULL);
        exit(0);
    }

    while(wait(NULL) > 0);
    // déplacement de la ligne du "salut" et du write aux lignes 22-23
    printf("Process completed!\n");
    close(fd[0]);
    close(fd[1]);
    return 0;
}