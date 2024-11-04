#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>


sem_t sem_occupe;// Indique le nombre d'éléments dans le tampon
sem_t sem_libre;// Indique les espaces libres dans le tampon
sem_t mutex;// Accès exclusif au tampon

int *tampon;
int taille_tampon;
int index_production = 0; 
int index_consommation = 0; 

int somme_produits = 0;
int somme_consommés = 0;
int total_produits = 0;
int total_consommés = 0;

bool flag_de_fin = false;

void gestionnaire_signal(int sig) {
    flag_de_fin = true;
}

void* producteur(void* pid) {
    int id = *(int*)pid;
    int somme_locale = 0;

    while (1) {

        int chiffre = (rand() % 9) + 1;
        somme_locale += chiffre;

        sem_wait(&sem_libre);            
        sem_wait(&mutex);         

        tampon[index_production] = chiffre; 
        index_production = (index_production + 1) % taille_tampon;
        total_produits++;

        sem_post(&mutex);         
        sem_post(&sem_occupe);            

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
        sem_wait(&sem_occupe);            
        sem_wait(&mutex);        

        int chiffre = tampon[index_consommation];  
            if (chiffre == 0) {
                sem_post(&mutex);      
                sem_post(&sem_libre);           
                break; 
            }
        index_consommation = (index_consommation + 1) % taille_tampon;
        total_consommés++;

        sem_post(&mutex);         
        sem_post(&sem_libre);          

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

    sem_init(&sem_occupe, 0, 0);              
    sem_init(&sem_libre, 0, taille_tampon);    
    sem_init(&mutex, 0, 1);         

    srand(time(NULL));
    
    signal(SIGALRM, gestionnaire_signal);

    for (int i = 0; i < nb_producteurs; i++) {
        pthread_create(&producteurs[i], NULL, producteur, &i);
    }

    for (int i = 0; i < nb_consommateurs; i++) {
        pthread_create(&consommateurs[i], NULL, consommateur, &i);
    }

    alarm(1);

    int somme_totale_produits = 0;
    for (int i = 0; i < nb_producteurs; i++) {
        void* somme_locale;
        pthread_join(producteurs[i], &somme_locale);
        somme_totale_produits += (int)(long)somme_locale;
    }

    for (int i = 0; i < nb_consommateurs; i++) {
        sem_wait(&sem_libre);
        sem_wait(&mutex);

        tampon[index_production] = 0;
        index_production = (index_production + 1) % taille_tampon;

        sem_post(&mutex);
        sem_post(&sem_occupe);
    }

    int somme_totale_consommés = 0;
    for (int i = 0; i < nb_consommateurs; i++) {
        void* somme_locale;
        pthread_join(consommateurs[i], &somme_locale);
        somme_totale_consommés += (int)(long)somme_locale;
    }

    printf("Somme des chiffres produits: %d\n", somme_totale_produits);
    printf("Somme des chiffres consommés: %d\n", somme_totale_consommés);
    printf("Nombre de chiffres produits: %d\n", total_produits);
    printf("Nombre de chiffres consommés: %d\n", total_consommés);

    free(tampon);
    sem_destroy(&sem_occupe);
    sem_destroy(&sem_libre);
    sem_destroy(&mutex);

    return 0;
}
