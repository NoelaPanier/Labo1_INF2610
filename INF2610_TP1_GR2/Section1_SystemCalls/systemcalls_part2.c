/*
 * Ecole Polytechnique Montreal - GIGLL
 * Automne 2024
 * SystemCalls - part2.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/

#include <stdio.h>
#include <stdlib.h>


void part21 ()
{
    char str[] = "77dbcb01f571f1c32g196c3a7d27f62e (printed using printf)";
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%c", str[i]);
    }
    write(1,"77dbcb01f571f1c32g196c3a7d27f62e (printed using write)\n",55);
}

void part22 ()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    part21();
}

int main (int argc, char* argv[])
{
    
    if (argc!=2)
    {   printf("Le programme a un seul paramètre : 1 ou 2\n");
        return 1;
    }
    switch (atoi(argv[1])) {
         case 1:        part21();
                        break;
         case 2:        part22();
                        break;
        default:        printf(" Le paramètre du programme est invalide\n");
                        return 1;
    }
    return 0;
}
