/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Challenges - part1.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/
#include "challenges_part1.h"

int compter_fichiers_txt(const char *dirpath, FILE *fichier_sortie){
    int nb_total_fichiers_txt = 0;
    dirent *entree;

    DIR *dir = opendir(dirpath);
    if( dir == NULL){
        perror("erreur à l'ouverture du dossier");
        return -1;
    }

    while((entree = readdir(dir)) != NULL) {
        if (strcmp(entree->d_name, ".") == 0 || strcmp(entree->d_name, "..") == 0) {
            continue;
        }

        if(entree->d_type == 4){    // 4 c'est pour un repertoire
            pid_t pid;
            if((pid=fork()) == 0){    // processus enfant
                char chemin_complet[MAX_PATH_LENGTH];
                snprintf(chemin_complet, sizeof(chemin_complet), "%s/%s", dirpath, entree->d_name);

                fprintf(fichier_sortie, "Emplacement du répertoire: %s\n", chemin_complet);
                fprintf(fichier_sortie, "Numéro d'identification du répertoire: %llu\n", (unsigned long long) entree->d_ino);


                exit(compter_fichiers_txt(chemin_complet, fichier_sortie));
            }
            else if(pid > 0){
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)){

                    nb_total_fichiers_txt += WEXITSTATUS(status);
                }
            }
        }
        else if(entree->d_type == 8){ // 8 c'est pour un fichier
            fprintf(fichier_sortie, "Liste des fichiers .txt: %s\n", entree->d_name);
            fprintf(fichier_sortie, "-------------------------------------------\n");
            nb_total_fichiers_txt++;
        }
        else{
            fprintf(fichier_sortie, "Aucun fichier .txt trouvés.");
            fprintf(fichier_sortie, "-------------------------------------------\n");
        }
    }

    closedir(dir);
    return nb_total_fichiers_txt;

}

int main(int argc, char*argv[])
{
    FILE *fichier_sortie = fopen("challenges_output.txt", "w");
    if (fichier_sortie == NULL) {
        perror("Erreur à l'ouverture du fichier de sortie");
        return 1;
    }
    int nb_total_fichiers_txt = compter_fichiers_txt("./root", fichier_sortie);
    printf("Nombre total de fichiers texte: %d\n", nb_total_fichiers_txt);

    fclose(fichier_sortie);
    return 0;
}



