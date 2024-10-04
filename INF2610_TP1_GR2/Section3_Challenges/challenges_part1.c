/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Challenges - part1.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/
#include "challenges_part1.h"

int compter_fichiers_txt(const char *dirpath, FILE *fichier_sortie, long unsigned int *parent_ino){
    int nb_total_fichiers_txt = 0;
    int nb_fichiers_txt = 0;
    dirent *entree;
    char **fichiers_txt = malloc(MAX_PATH_LENGTH * sizeof(char *));
    char *chemin_print = "";
    static long unsigned int *entree_d_ino_print = 0;

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
            if((pid=fork()) == 0){    
                char chemin_complet[MAX_PATH_LENGTH];
                snprintf(chemin_complet, sizeof(chemin_complet), "%s/%s", dirpath, entree->d_name);
                entree_d_ino_print = (long unsigned int*)entree->d_ino;

                exit(compter_fichiers_txt(chemin_complet, fichier_sortie, (long unsigned int*) entree->d_ino));
            }
            else if(pid > 0){
                int status;
                entree_d_ino_print = (long unsigned int*)entree->d_ino;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)){
                    nb_total_fichiers_txt += WEXITSTATUS(status);
                }
            }
        }
        else if(entree->d_type == 8){ // 8 c'est pour un fichier
            if (strstr(entree->d_name, ".txt") != NULL) {
                fichiers_txt[nb_fichiers_txt] = malloc(strlen(entree->d_name) + 1);
                strcpy(fichiers_txt[nb_fichiers_txt], entree->d_name);
                nb_fichiers_txt++;
                nb_total_fichiers_txt++;
            }
        }
    }

   
    char chemin_complet[MAX_PATH_LENGTH];
    snprintf(chemin_complet, sizeof(chemin_complet), "%s", dirpath);
    fprintf(fichier_sortie, "-----------------------------------------------------------------\n");
    fprintf(fichier_sortie, "Emplacement du répertoire: %s\n", chemin_complet);
    fprintf(fichier_sortie, "Numéro d'identification du répertoire: %llu\n",(unsigned long long)entree_d_ino_print);
    fprintf(fichier_sortie, "Numéro d'identification du répertoire supérieur immédiat: %llu\n", (unsigned long long)parent_ino);
    fprintf(fichier_sortie, "Liste des fichiers texte: \n");
    if (nb_fichiers_txt == 0) {
        fprintf(fichier_sortie, "Ce repertoire ne contient aucun fichier texte. \n");
    } else {
        for (int i = 0; i < nb_fichiers_txt; i++) {
            fprintf(fichier_sortie, "%s\n", fichiers_txt[i]);
            free(fichiers_txt[i]); 

        }
    }

    free(fichiers_txt); 

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
    int nb_total_fichiers_txt = compter_fichiers_txt("./root", fichier_sortie, NULL);
    printf("Nombre total de fichiers texte: %d\n", nb_total_fichiers_txt);

    fclose(fichier_sortie);
    return 0;
}



