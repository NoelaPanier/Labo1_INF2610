#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define ENTRY_CODE 7
#define N_THREADS 5

sem_t sem_critical;

// Question 1:
// 1. Exclusion mutuelle:
// 2. Detention et attente:
// 3. Pas de requisition:
// 4. Attente circulaire:

// Question 2: Le code ne représente pas une situation commune vu dans le cours.

// Question 3: L'ajout de la ligne 30, permet de libérer le sémaphore si key n'égale pas à ENTREY_CODE. En effet, cela permet
// d'éviter l'attente circulaire, car sans cette ligne, dans le cas où le if est false pour tous les threads, le sémaphore 
// ne sera jamais libéré.

int try_lock(int key){
    if(key == ENTRY_CODE){
        printf("Door unlocked\n");
        return 1;
    }
    sem_post(&sem_critical);  // ajout
    sleep(1);
    return 0;
}

void* door(void* args){
    while(1){
        sem_wait(&sem_critical);
        int key = rand() % 30;
        if(try_lock(key))
            pthread_exit(NULL);
        sem_post(&sem_critical);
    }
}
int main() {

    sem_init(&sem_critical, 0, 1); 
    pthread_t threads[N_THREADS];

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, door, NULL);
    }

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}