// Biblioteca do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "lista.h"
#include "lef.h"
#include "libTB.h"


int aleat(int min, int max) {

  return rand() % (max - min + 1) + min;
}

struct world_t *world_create ( long tstart, int wsize, int nskills,int nheroes,
                               int nbases, long nmiss) {

  struct world_t *w ; 

  w = malloc(sizeof(struct world_t)) ;

  w->NHeroes = nheroes ;
  w->NBases = nbases ;
  w->NMiss = nmiss ;
  w->WSize = wsize ;
  w->clock = tstart ;

  //seta os vetores 
  w->Heroes = malloc(sizeof(struct hero_t * nheros)) ;
  w->Bases = malloc(sizeof(struct base_t * nbases)) ;
  w->Miss = malloc(sizeof(struct miss_t * nmiss)) ;

  Heroes_create(w->Heroes, nheroes, nskills) ;
  Bases_create(w->Bases, wsize, nbases, nheroes) ;
  Miss_create(w->Miss, nmiss, wsize) ;

  if (!w->Heroes || !w->Bases || !w->Miss)
    return -1 ;
  
  return w ;
}  

//funcao exclusiva deste arquivo
int Heroes_create (struct hero_t *h, int nheroes, int nskills) {

  int i ; 

  if (!h)
    return -1 ;

  for (i = 0; i < nheroes; i++) {

    h[i]->Skills = set_create (nskills) ;
    //laço para evitar repetiçoes nas habilidades
    while( set_card(h[i]->Skills) < aleat(1, 3)) 
      set_add (h[i]->Skills, aleat(0, 9)) ;

    h[i]->id = i ; 
    h[i]->patience = aleat(0, 100) ;  
    h[i]->speed = aleat(50, 5000) ; // m/min 
    h[i]->xp = 0 ;
    //BaseId nao inicializado
  }
  return 1 ;
}

//funcao exclusiva deste arquivo
int Bases_create (struct base_t *b, int wsize, int nbases, int nheroes) {

  int i ;

  if (!b)
    return -1 ;

  for (i = 0; i < nbases; i++) {

    b->party = set_create(nheroes) ;
    b->cx = aleat(0, wsize -1) ;
    b->cy = aleat(0, wsize -1) ;
    b->wait = lista_cria() ;
  }
  return 1 ; 
}

//funcao exclusiva deste arquivo
int Miss_create (struct miss_t *m, int nmiss, int wsize) {

  int i ;

  if (!m)
    return -1 ;

  for (i = 0; i < nmiss; i++) {

    m->id = aleat(0, nmiss -1) ;
    m->cx = aleat(0, wsize -1) ;
    m->cy = aleat(0, wsize -1) ;
    m->skills = set_create(aleat(6, 10)) ;
  }
  return 1 ;
}










