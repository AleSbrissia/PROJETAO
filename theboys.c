// programa principal do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "lista.h"

// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 6
#define N_MISSOES T_FIM_DO_MUNDO / 100

// programa principal
int main () {

  struct world_t *w ;
  struct evento_t *ev ;

  // iniciar o mundo
  w = world_create(T_INICIO, N_TAMANHO_MUNDO, N_HABILIDADES, N_HEROIS,
                   N_BASES, N_MISSOES) ;

  // executar o laço de simulação
  while (w->clock <= T_FIM_DO_MUNDO) {

    ev = retira_lef(w->lef) ; 

  }

  // destruir o mundo
  w = world_destroy(w) ;

  return (0) ;
}

