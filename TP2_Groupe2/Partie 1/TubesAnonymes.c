#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int fd1[2];
    int fd2[2];
    pipe(fd1);  // Pipe pour le premier rev
    pipe(fd2);  // Pipe pour le second rev

    // Fork pour le premier processus (rev < In.txt)
    if(fork() == 0){
        close(fd2[0]); // Fermer les fd non utilises
        close(fd2[1]);
        printf("Processus 1: Inverser le contenu de In.txt avec rev\n");
        int file = open("In.txt", O_RDONLY);
        if (file == -1) {
            perror("Error opening In.txt");
            _exit(1);
        }
        close(fd1[0]);  // Fermer l'extrÃ©mitÃ© de lecture du pipe
        dup2(file, 0);   // Rediriger l'entrÃ©e standard vers In.txt
        dup2(fd1[1], 1); // Rediriger la sortie standard vers fd1[1]
        close(fd1[1]);
        close(file);

        execlp("rev", "rev", NULL);  // Lancer la commande rev
        perror("execlp rev failed");  // Si execlp Ã©choue
        _exit(1);
    }

    // Fork pour le deuxiÃ¨me processus (rev)
    if(fork() == 0) {
        printf("Processus 2: Inverser Ã  nouveau avec rev\n");
        close(fd1[1]);  // Fermer l'extrÃ©mitÃ© d'Ã©criture du premier pipe
        dup2(fd1[0], 0); // Rediriger l'entrÃ©e standard vers fd1[0]
        close(fd1[0]);
        close(fd2[0]);  // Fermer l'extrÃ©mitÃ© de lecture du second pipe
        dup2(fd2[1], 1); // Rediriger la sortie standard vers fd2[1]
        close(fd2[1]);

        execlp("rev", "rev", NULL);  // Lancer la commande rev
        perror("execlp rev failed");
        _exit(1);
    }

    // Fork pour le troisiÃ¨me processus (diff)
    if(fork() == 0){
        close(fd1[0]); // Fermer les fd non utilises
        close(fd1[1]);
        printf("Processus 3: Comparer les fichiers avec diff\n");
        close(fd2[1]);  // Fermer l'extrÃ©mitÃ© d'Ã©criture du second pipe
        dup2(fd2[0], 0); // Rediriger l'entrÃ©e standard vers fd2[0]
        close(fd2[0]);

        // VÃ©rifier si le fichier In.txt existe bien
        if (access("In.txt", F_OK) == -1) {
            perror("In.txt not found");
            _exit(1);
        }

        // Lancer la commande diff
        printf("Lancement de diff...\n");
        execlp("diff", "diff", "-", "In.txt", "-s", NULL);  // Lancer la commande diff
        perror("execlp diff failed");
        _exit(1);
    }

    // Fermer tous les descripteurs de fichiers dans le processus pÃ¨re
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    close(1);

    // Attendre la fin de tous les processus fils
    while (wait(NULL) > 0);

    return 0;
}
