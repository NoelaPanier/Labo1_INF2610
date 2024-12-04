#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define N_THREADS 5

int flag = 0;

// Question 1:
// 1. Exclusion mutuelle: Ne s'applique pas.
// 2. Detention et attente: Ne s'applique pas.
// 3. Pas de requisition: Ne s'applique pas.
// 4. Attente circulaire: Ne s'applique pas.

// Question 2: Le code ne représente pas une situation commune vu dans le cours.

// Question 3: La condition à la ligne 32 est impossible, donc flag ne pouvait jamais se mettre à 1. Alors, le programme ne 
// s'arrêter jamais.

void* function_that_does_something(void* arg) {
    while (1) {
        if (flag)
            break;
    }
    pthread_exit(NULL);
}

void changeFlag(){
    //if (rand() % 10 == 10)  on enlève cette condition car elle est impossible
    flag = 1;
    printf("Signal called\n");
}

int main() {
    pthread_t threads[N_THREADS];
 
    signal(SIGALRM, &changeFlag);      

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, function_that_does_something, NULL);
    }

    alarm(1);

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}