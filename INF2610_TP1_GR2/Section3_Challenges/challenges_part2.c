/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Challenges - part2.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/
#include "challenges_part2.h"

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

    for (int i=0;i<A->rows;i++){
        for (int j=0;j<B->cols;j++){
            result->matrix[i][j]=0;
            for (int k=0;k<B->rows;k++){
                result->matrix[i][j]+=A->matrix[i][k]*B->matrix[k][j];
            }    
        }
    }
    return result;
}


// This main is to help you in your development. 
// Don't forget to comment it out when you run the tests.
// int main(int argc, char*argv[])
// {
   
//     return 0;
// }
