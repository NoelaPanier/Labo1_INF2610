/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * SystemCalls - part1.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
    int fd = open("systemcalls_output2.txt", O_WRONLY | O_TRUNC);
 
    if (fd == -1){
        perror("Appel système open a échoué");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO,"Saisissez votre texte suivi de CTRL-D :\n", 40);

    char buffer;
    ssize_t bytesRead;

    while ((read(STDIN_FILENO, &buffer, 1)) != 0) {  // car CTRL-D retourne la valeur 0
        write(fd, &buffer, 1);
    }
   
    close(fd);

    return 0;
}
