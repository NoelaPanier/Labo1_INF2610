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
    int fd = open("systemcalls_output2.txt",O_WRONLY | O_TRUNC);
    ssize_t bytesRead;
    if (fd == -1){
        printf("Appel système open a échoué");
        exit(1);
    }
    printf("Saisissez votre texte suivi de CTRL-D :\n");
    int fs = open(0, O_RDONLY);
/*     char buffer;
    while (buffer!="ˆD"){
        read(fs,buffer,bytesRead);
    } */
    // TODO
    return 0;
}
