// programa principal do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

// seus #includes vão aqui
#include <stdio.h>
#include "ev.h"
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
  int try ;
  struct lef_t *lef ;
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

struct world_t *world_create (int tstart, int wsize, int nskills,int nheroes,
                              int nbases, long nmiss) ;

struct world_t *world_destroy (struct world_t *w) ;

// coloca os eventos iniciais
int world_start (struct world_t *w, long tend) ;

void world_loop (struct world_t *w) ;

//funcao para testes
void imprime_t (struct world_t *w) ;





