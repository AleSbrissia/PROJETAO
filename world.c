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

// defines de eventos 
#define EV_CHEGA 1
#define EV_ESPERA 2
#define EV_DESISTE 3
#define EV_AVISA 4
#define EV_ENTRA 5
#define EV_SAI 6
#define EV_VIAJA 7
#define EV_MISSAO 8
#define EV_FIM 9 

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
    h[i].speed = aleat(50, 5000) ; // em m/min 
    h[i].xp = 0 ;
    //BaseId nao inicializado ainda
  }
  return h ;
}

//funcao exclusiva deste arquivo
struct base_t *Bases_create (int wsize, int nheroes, int nbases, int nskills) {

  int i ;
  struct base_t *b ;
  
  b = malloc(sizeof(struct base_t) * nbases) ;
  if (!b)
    return NULL ;

  for (i = 0; i < nbases; i++) {

    b[i].id = i ;
    b[i].size = aleat(3, 10) ;
    b[i].party = set_create(nheroes) ;
    b[i].cx = aleat(0, wsize -1) ;
    b[i].cy = aleat(0, wsize -1) ;
    b[i].wait = lista_cria() ;
    b[i].skills = set_create(nskills) ;
  }
  return b ; 
}

// funcao exclusiva deste arquivo
// aloca um vetor de missoes e inicializa os conjuntos de habilidades
struct miss_t *Miss_create (int nmiss, int wsize, int nskills) {

  int i, tam ;
  struct miss_t *m ;

  m = malloc(sizeof(struct miss_t) * nmiss) ;
  if (!m)
    return NULL ;

  for (i = 0; i < nmiss; i++) {

    m[i].id = i ;
    m[i].cx = aleat(0, wsize -1) ;
    m[i].cy = aleat(0, wsize -1) ;
    m[i].skills = set_create(nskills) ;
    tam = aleat(6, 10) ;

    //laço para evitar repetiçoes nas habilidades
    while( set_card(m[i].skills) < tam) 
      set_add (m[i].skills, aleat(0, 9)) ;

  }
  return m ;
}

struct world_t *world_create (int tstart, int wsize, int nskills,int nheroes,
                              int nbases, int nmiss, int endtime) {

  struct world_t *w ; 

  w = malloc(sizeof(struct world_t)) ;

  w->NHeroes = nheroes ;
  w->NBases = nbases ;
  w->NMiss = nmiss ;
  w->WSize = wsize ;
  w->clock = tstart ;
  w->EndTime = endtime ;
  w->lef = cria_lef() ;
  w->cont = 0 ; 
  w->tr = nmiss ; //no mínimo um agendamento por missao

  //seta os vetores 
  w->Heroes = Heroes_create(nheroes, nskills) ;
  w->Bases = Bases_create(wsize, nheroes, nbases, nskills) ;
  w->Miss = Miss_create(nmiss, wsize, nskills) ;

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
    set_destroy (w->Bases[i].skills) ;
    lista_destroi (w->Bases[i].wait) ;
  }

  for (i = 0; i < w->NMiss ; i++) 
    set_destroy (w->Miss[i].skills) ;
 
  destroi_lef(w->lef) ;
  free(w->Heroes) ;
  free(w->Bases) ;
  free(w->Miss) ;
  free(w) ;

  return NULL ;
}

int world_start (struct world_t *w, long tend) {

  int i ;
  struct evento_t *ev ;

  if (!w || !w->Heroes || !w->Miss || !w->Bases) 
    return 0 ;

  for (i = 0; i < w->NHeroes; i++) {
    
    w->Heroes[i].BaseId = aleat(0, w->NBases -1) ;
    ev = cria_evento(aleat(0, 4320), 1, w->Heroes[i].id, w->Heroes[i].BaseId) ;
    insere_lef(w->lef, ev) ;
  }

  for (i = 0; i < w->NMiss; i++) {
    
    ev = cria_evento (aleat(0 , tend), 8, w->Miss[i].id, 0) ; //d2 nao precisa
    insere_lef (w->lef, ev) ;
  }
  
  ev = cria_evento (tend, 9, 0, 0) ;
  insere_lef (w->lef, ev) ;

  return 1 ;
}

void imprime_t (struct world_t *w) { //FUNCAO PARA TESTE

  int i ;
  char n ;

  n = 'W' ;

  printf("HEROIS\n") ;
  for (i = 0; i < w->NHeroes; i++) {
    
    printf("id %2d, pac %3d, spe %4d, Bid %d ", w->Heroes[i].id,
            w->Heroes[i].patience, w->Heroes[i].speed, w->Heroes[i].BaseId) ;
    set_print(w->Heroes[i].Skills) ;
    printf("\n") ;
  }

  printf("BASES:\n") ;
  for (i = 0; i < w->NBases ; i++) {
    
    printf("id %d, size %2d, cx %5d, cy %5d ", w->Bases[i].id,
          w->Bases[i].size, w->Bases[i].cx, w->Bases[i].cy) ; 

    set_print(w->Bases[i].party) ;
    set_print(w->Bases[i].skills) ;
    lista_imprime(&n, w->Bases[i].wait) ;
  }

  printf("Missoes: \n") ;
  for (i = 0; i < w->NMiss ; i++) {
    
    printf("MISSAO id %d, cx %d, cy %d ", w->Miss[i].id, w->Miss[i].cx,w->Miss[i].cy) ;
    set_print(w->Miss[i].skills) ;
    printf("\n") ;
  } 
}

void world_loop (struct world_t *w) {

  struct evento_t *ev ;

  if (!w)
    return ;

  while (w->clock < w->EndTime) {

    ev = retira_lef(w->lef) ; 
    w->clock = ev->tempo ;

    switch (ev->tipo) {

      case EV_CHEGA :
       
        trata_evento_chega(w, ev) ;
        break ;

      case EV_ESPERA :

        trata_evento_espera(w, &w->Heroes[ev->dado1], &w->Bases[ev->dado2]) ;
        break ;

      case EV_DESISTE :

        trata_evento_desiste(w, ev) ;
        break ;

      case EV_AVISA :

        trata_evento_avisa(w, &w->Bases[ev->dado2]) ;
        break ;

      case EV_ENTRA :

        trata_evento_entra(w, &w->Heroes[ev->dado1], &w->Bases[ev->dado2]) ;
        break ;

      case EV_SAI :

        trata_evento_sai(w, ev) ;
        break ;

      case EV_VIAJA :

        trata_evento_viaja(w, &w->Heroes[ev->dado1], &w->Bases[ev->dado2]) ;
        break ;

      case EV_MISSAO :

        trata_evento_missao(w, ev) ;
        break ;

      case EV_FIM :
        
        trata_evento_fim(w, ev) ;
        break ;
    }
    destroi_evento(ev) ; 
  } 
}

