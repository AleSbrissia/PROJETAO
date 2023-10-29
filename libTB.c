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

//funcao exclusiva deste arquivo
struct hero_t *Heroes_create (int nheroes, int nskills) {

  int i, tam ;
  struct hero_t *h ;

  h = malloc(sizeof(struct hero_t) * nheroes) ;
  if (!h)
    return NULL ;

  for (i = 0; i < nheroes; i++) {

    h[i].Skills = set_create (nskills) ;
    tam = aleat(1, 3) ;

    //laço para evitar repetiçoes nas habilidades
    while( set_card(h[i].Skills) < tam) 
      set_add (h[i].Skills, aleat(0, 9)) ;

    h[i].id = i ; 
    h[i].patience = aleat(0, 100) ;  
    h[i].speed = aleat(50, 5000) ; // m/min 
    h[i].xp = 0 ;
    //BaseId nao inicializado
  }
  return h ;
}

//funcao exclusiva deste arquivo
struct base_t *Bases_create (int wsize, int nbases, int nheroes) {

  int i ;
  struct base_t *b ;
  
  b = malloc(sizeof(struct base_t) * nbases) ;
  if (!b)
    return NULL ;

  for (i = 0; i < nbases; i++) {

    b[i].party = set_create(nheroes) ;
    b[i].cx = aleat(0, wsize -1) ;
    b[i].cy = aleat(0, wsize -1) ;
    b[i].wait = lista_cria() ;
  }
  return b ; 
}

//funcao exclusiva deste arquivo
//cria um vetor de struct e inicializa os conjuntos e listas
struct miss_t *Miss_create (int nmiss, int wsize) {

  int i ;
  struct miss_t *m ;

  m = malloc(sizeof(struct miss_t) * nmiss) ;
  if (!m)
    return NULL ;

  for (i = 0; i < nmiss; i++) {

    m[i].id = aleat(0, nmiss -1) ;
    m[i].cx = aleat(0, wsize -1) ;
    m[i].cy = aleat(0, wsize -1) ;
    m[i].skills = set_create(aleat(6, 10)) ;
  }
  return m ;
}

struct world_t *world_create (long tstart, int wsize, int nskills,int nheroes,
                              int nbases, long nmiss) {

  struct world_t *w ; 

  w = malloc(sizeof(struct world_t)) ;

  w->NHeroes = nheroes ;
  w->NBases = nbases ;
  w->NMiss = nmiss ;
  w->WSize = wsize ;
  w->clock = tstart ;
  w->lef = cria_lef() ;

  //seta os vetores 
  w->Heroes = Heroes_create(nheroes, nskills) ;
  w->Bases = Bases_create(wsize, nbases, nheroes) ;
  w->Miss = Miss_create(nmiss, wsize) ;

  if (!w->Heroes || !w->Bases || !w->Miss)
    return NULL ;
  
  return w ;
}  

struct world_t *world_destroy (struct world_t *w) {

  int i ;

  if(!w)
    return NULL ;

  for (i = 0 ; i < w->NHeroes ; i++) 
    set_destroy (w->Heroes[i].Skills) ;

  for (i = 0 ; i < w->NBases ; i++) {

    set_destroy (w->Bases[i].party) ;
    lista_destroi (w->Bases[i].wait) ;
  }

  for (i = 0; i < nmiss ; i++) 
    set_destroy (w->Miss[i].skills) ;
 
  destroi_lef(w->lef) ;
  free(w->Heroes) ;
  free(w->Bases) ;
  free(w->Miss) ;
  free(w) ;

  return NULL ;
}

struct evento_t *evento_fim (struct world_t *w, long tend) {

  int i ;
  struct evento_t *ev ;

  if (!w || !w->Heroes) 
    return NULL ;

  printf("%ld: FIM\n", tend) ;

  for (i = 0 ; i < w->NHeroes ; i++) {

    printf("HEROI %d PAC %d VEL %d EXP %d HABS", w->Heroes[i].id, 
            w->Heroes[i].patience, w->Heroes[i].speed, w->Heroes.xp) ;
    set_print(w->Heroes) ;
  }  

  

}

int world_start (struct world_t *w, long tend, int nbases) {

  int i ;

  for (i = 0; i < nbases; i++) {


































