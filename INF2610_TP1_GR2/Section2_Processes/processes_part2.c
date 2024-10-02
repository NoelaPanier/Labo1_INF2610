/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Processes - part2.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/

#include "./libprocesslab/libprocesslab.h"

#define m 1000000
// question2 calcule la somme des m premiers nombres naturels 1+2+...+m

// nb est le nombre de threads qui vont contribuer au calcul
#define nb 4

//tableau somme est utilisé pour le calcul des sommes patielles par les threads
long somme[nb];


// fonction exécutée par chaque thread créé
void* contribution(void*p)
{
   int no = *((int*)p);
   int intervalle1 = (no * (m/nb)) + 1;
   int intervalle2 = (no+1) *(m/nb);

  for(int i=intervalle1;i<=intervalle2;i++){
    somme[no]+=i;
  }
    
  return NULL;
}


void question2( )
{
  pthread_t *tid = malloc (nb * sizeof(pthread_t));

  int num[nb];
  for(int i=0; i<nb; i++){
    num[i] = i;
  }

  long somme_contribution = 0;
  long vrai_somme = 0;

  for(int i=0; i<nb; i++){
    pthread_create(&tid[i], NULL, contribution, (void*) &num[i]);
  }

  for(int i=0; i<nb; i++){
    pthread_join(tid[i], NULL);
  }

  for(int i=0; i<nb; i++){
    somme_contribution += somme[i];
  }

  vrai_somme = (long)m * ((long)m + 1)/2;
  printf("somme contribution: %ld, vrai somme: %ld \n", somme_contribution, vrai_somme);
    
}

