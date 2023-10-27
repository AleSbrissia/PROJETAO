// Biblioteca do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "lista.h"
#include "libTB.h"


int aleat(int min, int max) {

  return rand() % (max - min + 1) + min;
}

int world_create (struct world_t *w, long tend, int wsize, int nskills,
                  int nheroes, int nbases, long nmiss) {
 
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

  //T: TERMINAR DE SETAR OS VETORES!!


  if (!w->Heroes || !w->Bases || !w->Miss)
    return -1 ;

}  

int Heroes_create (struct hero_t *h, int nheroes, int nskills, int nbases) {

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
    h[i]->BaseId =  ; //PERGUNTAR
  }
}

int Bases_create (struct world_t *w, int nbases, int nheroes) {

  if (!b)
    return -1 ;

  w->Bases->party = set_create(nheroes) ;
   
















