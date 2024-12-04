#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 5
#define MAX_ROUNDS 3

pthread_mutex_t locks[NUM_THREADS];

// Question 1:
// 1. Exclusion mutuelle: L'exclusion mutuelle est représentée par les pthread_mutex aux lignes 32-43.
// 2. Detention et attente: La detention et attente est représentée aux lignes 32 et 33, car il se peut que le philosophe détienne la fourchette de gauce et attend pour avoir celle de droite.
// 3. Pas de requisition: Lorsqu'un philosophe détient une fourchette, elle ne peut être libérée que par lui-même. Cela peut se voir aux lignes 42 et 43.
// 4. Attente circulaire: Si chaque philosophe prend sa fourchette de gauche et veut celle de droite, cela va créer une attente circulaire. On peut voir ça à la ligne 33.

// Question 2: Ce code représente le problème des philosophes

// Question 3: L'ajout du if else représente la 2eme solution au problème des philosophes vu dans le cours. En effet, 
// la condition (left<right) permet d'enlever l'attente circulaire, car elle permet d'avoir un ordre fixe d'acquision des verrous.
// Par exemple, cette condition empêche le cas où tous les threads prennent left en premier et cause un interbloquage.

void* cons(void* arg) {
    int id = *(int*)arg;
    int rounds = 0;

    while (rounds < MAX_ROUNDS) {
        sleep(1);
        int left = id;
        int right = (id + 1) % NUM_THREADS;

        if(left < right){                       // ajout
            pthread_mutex_lock(&locks[left]);
            pthread_mutex_lock(&locks[right]);
        }                                       // ajout
        else{                                   // ajout
            pthread_mutex_lock(&locks[right]);  // ajout
            pthread_mutex_lock(&locks[left]);   // ajout
        }

        sleep(1);
        rounds++;
        pthread_mutex_unlock(&locks[left]);
        pthread_mutex_unlock(&locks[right]);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_init(&locks[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, cons, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}