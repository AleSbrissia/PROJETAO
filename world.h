// Biblioteca Mundo do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

#ifndef _WORLD_H
#define _WORLD_H

// seus #includes vão aqui
#include <stdio.h>
#include "set.h"
#include "lista.h"
#include "lef.h"

struct world_t {

  int NHeroes ;
  struct hero_t *Heroes ;
  int NBases ;
  struct base_t *Bases ;
  int NMiss ;
  struct miss_t *Miss ;
  int NSkills ;
  int WSize ;
  int cont ;
  int tr ;
  struct lef_t *lef ;
  int EndTime ;
  int clock ;
} ;

struct base_t {

  int id ;
  int size ;
  struct set_t *party ;
  struct set_t *skills ;
  lista_t *wait ;
  int cx ;
  int cy ;
} ;

struct hero_t {

  int id ;
  struct set_t *Skills ;
  int patience ;
  int speed ;
  int xp ;
  int BaseId ;
} ;

struct miss_t {
 
  int id ;
  struct set_t *skills ;
  int cx ;
  int cy ;
} ;

int aleat(int min, int max) ;

/*aloca o mundo e seta as variaveis de acordo com os defines*/
struct world_t *world_create (int tstart, int wsize, int nskills, int nheroes,
                              int nbases, int nmiss, int endtime) ;  

struct world_t *world_destroy (struct world_t *w) ; 

/*eventos iniciais do mundo
  retorna 0 em caso de erro e 1 em sucesso*/
int world_start (struct world_t *w) ;

/*laço de simulacao do mundo*/
void world_loop (struct world_t *w) ;

#endif
