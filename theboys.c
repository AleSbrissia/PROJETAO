// programa principal do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "lista.h"
#include "libTB.h"

// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS "N_HABILIDADES * 5"
#define N_BASES "N_HEROIS / 6"
#define N_MISSOES "T_FIM_DO_MUNDO / 100"

// funções aqui



// programa principal
int main ()
{
  struct world_t *w ;

  // iniciar o mundo
  w = world_create(T_INICIO, N_TAMANHO_MUNDO, N_HABILIDADES, N_HEROIS, N_BASES,
                   N_MISSOES) ;


  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}

