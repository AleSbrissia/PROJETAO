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
#define N_HEROIS "N_HABILIDADES * 5"
#define N_BASES "N_HEROIS / 6"
#define N_MISSOES "T_FIM_DO_MUNDO / 100"

struct world_t {

  int NHero ;
  struct hero_t *Hero ;
  int NBase ;
  struct base_t *Base ;
  int NMiss ;
   Miss ;
  int NSkills ;
  int WSize ;
  long clock ;
}

struct base_t {

  int id ;
  int size ;
  int party ;
  struct lista_t *wait ;
  int cx ;
  int cy ;
}

struct hero_t {

  int id ;
  int Skills ;
  int patience ;
  int speed ;
  int xp ;
  int BaseId
}


// funções aqui

int aleat(int min, int max) {

  return rand() % (max - min + 1) + min; 
}

// programa principal
int main ()
{
  // iniciar o mundo

  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}

