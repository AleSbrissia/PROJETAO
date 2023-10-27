// Biblioteca do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "lista.h"

struct world_t {

  int NHeros ;
  struct hero_t *Heros ;
  int NBases ;
  struct base_t *Bases ;
  int NMiss ;
  struct miss_t Miss ;
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

struct miss_t {
 
  int id ;
  struct set_t *skills ;
  int cx ;
  int cy ;
}

int aleat(int min, int max) ;

/*
 *inicializa o mundo.
 *Ordem das entradas igual a dos defines em theboys.c
 *com excessao de T_FIM_DO_MUNDO
*/
int world_create (struct world_t *w, long tstart, int wsize, 
                  int nskills, int nheros, int nbases, long nmiss) ;
























