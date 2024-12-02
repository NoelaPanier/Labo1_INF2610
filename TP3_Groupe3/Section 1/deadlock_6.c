#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define N_THREADS 5

int flag = 0;

// Question 1:
// 1. Exclusion mutuelle:
// 2. Detention et attente:
// 3. Pas de requisition:
// 4. Attente circulaire:

// Question 2:

// Question 3:

void* function_that_does_something(void* arg) {
    while (1) {
        if (flag)
            break;
    }
    pthread_exit(NULL);
}

void changeFlag(){
    if (rand() % 10 == 10)
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