/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Challenges - part2.c
 *
 * Noëla Panier 2197449, Thomas Tzeng Souvanlasy 2214055
*/
#include "challenges_part2.h"

typedef struct {
    Matrix* A;
    Matrix* B;
    Matrix* result;
    int row;
} ThreadData;

// Thread function to perform matrix multiplication for a row
void* multiply_thread(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    Matrix* A = data->A;
    Matrix* B = data->B;
    Matrix* result = data->result;
    int row = data->row;

    // Perform matrix multiplication for the row
        for (int j = 0; j < B->cols; j++) {
            result->matrix[row][j] = 0;  // Initialize result element
            for (int k = 0; k < A->cols; k++) {
                result->matrix[row][j] += A->matrix[row][k] * B->matrix[k][j];
            }
        }

    pthread_exit(NULL);
}

Matrix* multiply(Matrix* A, Matrix* B) {
    if (A == NULL || B == NULL) {
        return NULL;
    }
    if (A->matrix == NULL || B->matrix == NULL) {
        return NULL;
    }
    if (A->rows == 0 || B->cols == 0|| A->cols == 0|| B->rows == 0) {
        return NULL;
    }
    if(A->cols!=B->rows){
        return NULL;
    }
    Matrix* result = malloc(sizeof(Matrix));
    if (result == NULL) {
        return NULL; // Failed to allocate memory for the result matrix
    }
    result->rows=A->rows;
    result->cols=B->cols;
    result->matrix = (short**)malloc(A->rows * sizeof(short*));
    if (result->matrix == NULL) {
        free(result); // Clean up the allocated memory
        return NULL; // Failed to allocate memory for rows
    }
    for (int i = 0; i < A->rows ; i++) {
        result->matrix[i] = (short*)malloc(B->cols * sizeof(short));
        if (result->matrix[i] == NULL) {
            // Clean up previously allocated rows
            for (int j = 0; j < i; j++) {
                free(result->matrix[j]);
            }
            free(result->matrix);
            free(result);
            return NULL; // Failed to allocate memory for columns
        }
    }
    // Create and initialize thread data
    pthread_t threads[A->rows];
    ThreadData thread_data[A->rows];

    for (int i=0;i<A->rows;i++){
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].result = result;
        thread_data[i].row = i;
        pthread_create(&threads[i], NULL, multiply_thread, (void*)&thread_data[i]);
    }
    for (int i = 0; i < A->rows; i++) {
        pthread_join(threads[i], NULL);
    }
    return result;
}


// This main is to help you in your development. 
// Don't forget to comment it out when you run the tests.
// int main(int argc, char*argv[])
// {
   
//     return 0;
// }
