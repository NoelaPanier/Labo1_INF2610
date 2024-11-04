#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

// Variables globales pour les sémaphores et les ressources partagées
sem_t sem_occupe;     // Indique le nombre d'éléments dans le tampon (consommateur attend)
sem_t sem_libre;      // Indique l'espace libre dans le tampon (producteur attend)
sem_t mutex;  // Accès exclusif au tampon

int *tampon;         // Tampon pour stocker les chiffres
int taille_tampon;

int somme_produits = 0;
int somme_consommés = 0;
int total_produits = 0;
int total_consommés = 0;

int ip = 0; // Position de production dans le tampon
int ic = 0; // Position de consommation dans le tampon

int flag_de_fin = 0; // Drapeau pour signaler la fin des producteurs

void gestionnaire_signal(int sig) {
    flag_de_fin = 1;
}

void* producteur(void* pid) {


    int somme_locale = 0;
    // ...
    while (1) {
        int x=(rand()%9) + 1;
        somme_locale += x;
        sem_wait(&sem_libre);            // Attend de l'espace dans le tampon
        sem_wait(&mutex);         // Accès exclusif au tampon

        tampon[ip] = x;  // Place le chiffre dans le tampon
        ip = (ip + 1) % taille_tampon;
        total_produits++;

        sem_post(&mutex);         // Libère le tampon
        sem_post(&sem_occupe); 
        if (flag_de_fin) {
            break; // Fin du thread producteur si flag_de_fin est true
        }
    } 
    return NULL;
}

void* consommateur(void* cid) {
    int somme_locale = 0;

    while (1) {
        sem_wait(&sem_occupe);            // Attend un élément dans le tampon
        sem_wait(&mutex);         // Accès exclusif au tampon

        int chiffre = tampon[ic];  // Récupère un chiffre du tampon
        ic = (ic + 1) % taille_tampon;
        total_consommés++;

        sem_post(&mutex);         // Libère le tampon
        sem_post(&sem_libre);             // Signale un espace libre

        somme_locale += chiffre;

        if (chiffre == 0) {
            break; // Fin du consommateur si le chiffre est 0
        }
    }

    pthread_exit((void*)(long)somme_locale);
    return NULL;
}
// ...

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int nb_producteurs = atoi(argv[1]);
    int nb_consommateurs = atoi(argv[2]);
    taille_tampon = atoi(argv[3]);
    // Les paramètres du programme sont, dans l'ordre :
    // le nombre de producteurs, le nombre de consommateurs
    // et la taille du tampon.
    pthread_t producteurs[nb_producteurs];
    pthread_t consommateurs[nb_consommateurs];

    // Initialisation des sémaphores
    sem_init(&sem_occupe, 0, 0);               // Aucun élément au début
    sem_init(&sem_libre, 0, taille_tampon);    // Tout l'espace est vide
    sem_init(&mutex, 0, 1);            // Mutex pour le tampon
    // ..
    // Gestionnaire de signal pour l'alarme
    signal(SIGALRM, gestionnaire_signal);

        // Création des threads producteurs
    for (int i = 0; i < nb_producteurs; i++) {
        pthread_create(&producteurs[i], NULL, producteur, &i);
    }

    // Création des threads consommateurs
    for (int i = 0; i < nb_consommateurs; i++) {
        pthread_create(&consommateurs[i], NULL, consommateur, &i);
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
        sem_wait(&sem_libre);
        sem_wait(&mutex);

        tampon[ip] = 0;
        ip = (ip + 1) % taille_tampon;

        sem_post(&mutex);
        sem_post(&sem_occupe);
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
    sem_destroy(&sem_occupe);
    sem_destroy(&sem_libre);
    sem_destroy(&mutex);
    return 0;
}
