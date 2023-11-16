// programa principal do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "set.h"
#include "lista.h"
#include "lef.h"
#include "world.h"

// defines no mundo:
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

  // iniciar o mundo
  srand(0) ;

  w = world_create(T_INICIO, N_TAMANHO_MUNDO, N_HABILIDADES, N_HEROIS,
                   N_BASES, N_MISSOES, T_FIM_DO_MUNDO) ;
  world_start(w, T_FIM_DO_MUNDO) ;

  //imprime_lef(w->lef) ;
  //imprime_t(w) ;

  // executar o laço de simulação
  world_loop(w) ;

  //imprime_t(w) ;

  // destruir o mundo
  w = world_destroy(w) ;

  return 0 ;
}



