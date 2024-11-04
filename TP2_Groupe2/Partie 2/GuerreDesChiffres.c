#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

// Variables globales pour les sémaphores et les ressources partagées
sem_t sem_plein;     // Indique le nombre d'éléments dans le tampon (consommateur attend)
sem_t sem_vide;      // Indique l'espace libre dans le tampon (producteur attend)
sem_t mutex_tampon;  // Accès exclusif au tampon

int *tampon;         // Tampon pour stocker les chiffres
int taille_tampon;
int index_production = 0; // Position de production dans le tampon
int index_consommation = 0; // Position de consommation dans le tampon

// Variables globales pour les statistiques
int somme_produits = 0;
int somme_consommés = 0;
int total_produits = 0;
int total_consommés = 0;

int flag_de_fin = 0; // Drapeau pour signaler la fin des producteurs

void gestionnaire_signal(int sig) {
    flag_de_fin = 1;
}

void* producteur(void* pid) {
    int id = *(int*)pid;
    int somme_locale = 0;

    srand(time(NULL));  // Initialisation du générateur aléatoire

    while (1) {

        int chiffre = (rand() % 9) + 1;  // Génère un chiffre non nul
        somme_locale += chiffre;

        sem_wait(&sem_vide);            // Attend de l'espace dans le tampon
        sem_wait(&mutex_tampon);         // Accès exclusif au tampon

        tampon[index_production] = chiffre;  // Place le chiffre dans le tampon
        index_production = (index_production + 1) % taille_tampon;
        total_produits++;

        sem_post(&mutex_tampon);         // Libère le tampon
        sem_post(&sem_plein);            // Signale la présence d'un élément

        // Vérifie la fin après chaque production
        if (flag_de_fin) {
            break;
        }
    
    }

    pthread_exit((void*)(long)somme_locale);
}

void* consommateur(void* cid) {
    int id = *(int*)cid;
    int somme_locale = 0;

    while (1) {
        sem_wait(&sem_plein);            // Attend un élément dans le tampon
        sem_wait(&mutex_tampon);         // Accès exclusif au tampon

        int chiffre = tampon[index_consommation];  // Récupère un chiffre du tampon
            if (chiffre == 0) {
                sem_post(&mutex_tampon);         // Libère le tampon
                sem_post(&sem_vide);  
                break; // Fin du consommateur si le chiffre est 0
            }
        index_consommation = (index_consommation + 1) % taille_tampon;
        total_consommés++;

        sem_post(&mutex_tampon);         // Libère le tampon
        sem_post(&sem_vide);             // Signale un espace libre


        somme_locale += chiffre;

    }

    pthread_exit((void*)(long)somme_locale);
}

int main(int argc, char* argv[]) {

    int nb_producteurs = atoi(argv[1]);
    int nb_consommateurs = atoi(argv[2]);
    taille_tampon = atoi(argv[3]);

    tampon = (int*)malloc(taille_tampon * sizeof(int));

    pthread_t producteurs[nb_producteurs];
    pthread_t consommateurs[nb_consommateurs];
    int ids_producteurs[nb_producteurs];
    int ids_consommateurs[nb_consommateurs];

    // Initialisation des sémaphores
    sem_init(&sem_plein, 0, 0);               // Aucun élément au début
    sem_init(&sem_vide, 0, taille_tampon);    // Tout l'espace est vide
    sem_init(&mutex_tampon, 0, 1);            // Mutex pour le tampon

    // Gestionnaire de signal pour l'alarme
    signal(SIGALRM, gestionnaire_signal);

    // Création des threads producteurs
    for (int i = 0; i < nb_producteurs; i++) {
        ids_producteurs[i] = i;
        pthread_create(&producteurs[i], NULL, producteur, &ids_producteurs[i]);
    }

    // Création des threads consommateurs
    for (int i = 0; i < nb_consommateurs; i++) {
        ids_consommateurs[i] = i;
        pthread_create(&consommateurs[i], NULL, consommateur, &ids_consommateurs[i]);
    }

    // Démarre l'alarme
    alarm(1);

    // Attente de la fin des threads producteurs
    int somme_totale_produits = 0;
    for (int i = 0; i < nb_producteurs; i++) {
        void* somme_locale;
        pthread_join(producteurs[i], &somme_locale);
        somme_totale_produits += (int)(long)somme_locale;
    }

    // Dépose des zéros pour chaque consommateur pour signaler la fin
    for (int i = 0; i < nb_consommateurs; i++) {
        sem_wait(&sem_vide);
        sem_wait(&mutex_tampon);

        tampon[index_production] = 0;
        index_production = (index_production + 1) % taille_tampon;

        sem_post(&mutex_tampon);
        sem_post(&sem_plein);
    }

    // Attente de la fin des threads consommateurs
    int somme_totale_consommés = 0;
    for (int i = 0; i < nb_consommateurs; i++) {
        void* somme_locale;
        pthread_join(consommateurs[i], &somme_locale);
        somme_totale_consommés += (int)(long)somme_locale;
    }

    // Affichage des résultats
    printf("Somme des chiffres produits: %d\n", somme_totale_produits);
    printf("Somme des chiffres consommés: %d\n", somme_totale_consommés);
    printf("Nombre de chiffres produits: %d\n", total_produits);
    printf("Nombre de chiffres consommés: %d\n", total_consommés);

    // Nettoyage
    free(tampon);
    sem_destroy(&sem_plein);
    sem_destroy(&sem_vide);
    sem_destroy(&mutex_tampon);

    return 0;
}
