// programa principal do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "lista.h"
#include "lef.h"

// seus #defines vão aqui

// defines no mundo:
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 6
#define N_MISSOES T_FIM_DO_MUNDO / 100

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

struct world_t {

  int NHeroes ;
  struct hero_t *Heroes ;
  int NBases ;
  struct base_t *Bases ;
  int NMiss ;
  struct miss_t *Miss ;
  int NSkills ;
  int WSize ;
  struct lef_t *lef ;
  int clock ;
} ;

struct base_t {

  int id ;
  int size ;
  struct set_t *party ;
  struct set_t *skills ;
  lista_t *wait ;
  int NHeroes ;
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

//funcoes:

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
struct base_t *Bases_create (int wsize, int nbases, int nheroes) {

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
    b[i].skills = set_create(N_HABILIDADES) ;
  }
  return b ; 
}

// funcao exclusiva deste arquivo
// aloca um vetor de missoes e inicializa os conjuntos de habilidades
struct miss_t *Miss_create (int nmiss, int wsize) {

  int i, tam ;
  struct miss_t *m ;

  m = malloc(sizeof(struct miss_t) * nmiss) ;
  if (!m)
    return NULL ;

  for (i = 0; i < nmiss; i++) {

    m[i].id = i ;
    m[i].cx = aleat(0, wsize -1) ;
    m[i].cy = aleat(0, wsize -1) ;
    m[i].skills = set_create(N_HABILIDADES) ;
    tam = aleat(6, 10) ;

    //laço para evitar repetiçoes nas habilidades
    while( set_card(m[i].skills) < tam) 
      set_add (m[i].skills, aleat(0, 9)) ;

  }
  return m ;
}

struct world_t *world_create (int tstart, int wsize, int nskills,int nheroes,
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
    
    w->Heroes[i].BaseId = aleat(0, w->NBases) ;
    ev = cria_evento(0, 1, w->Heroes[i].id, w->Heroes[i].BaseId) ;
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

int trata_evento_fim (struct world_t *w, struct evento_t *end) {

  int i ;

  if (!w || !w->Heroes) 
    return 0 ;

  printf("%d: FIM\n", end->tempo) ;

  for (i = 0 ; i < w->NHeroes ; i++) {

    printf("HEROI %d PAC %d VEL %d EXP %d HABS ", w->Heroes[i].id, 
            w->Heroes[i].patience, w->Heroes[i].speed, w->Heroes[i].xp) ;
  
    set_print(w->Heroes[i].Skills) ;
    printf("\n") ;
  }  

  //terminar 
  printf("/ MISSOES CUMPRIDAS (2f), MEDIA 2f TENTATIVAS/MISSAO\n") ;

  return 1 ;
} 

int trata_evento_chega(struct world_t *w, struct evento_t *ch) {

  struct evento_t *ev ;
  bool espera ;
  int d1, d2 ;

  if (!w || !w->lef || !w->Bases[ch->dado2].wait ) 
    return 0 ; 

  d1 = ch->dado1 ;
  d2 = ch->dado2 ;
    
  if (lista_vazia(w->Bases[d2].wait) && w->Bases[d2].NHeroes <
      w->Bases[d2].size) 
    espera = true ;
  else 
    espera = w->Heroes[d1].patience > (10 * lista_tamanho(w->Bases[d2].wait)) ;
    
  if (espera) {
  
    ev = cria_evento(w->clock, 2, w->Heroes[d1].id, w->Bases[d2].id) ;
    printf("%d: CHEGA  HEROI %d BASE %d (%d/%d) ESPERA\n", w->clock,
           w->Heroes[d1].id, w->Bases[d2].id, w->Bases[d2].NHeroes,
           w->Bases[d2].size) ;
    insere_lef(w->lef, ev) ;
  }
  else {
  
    ev = cria_evento(w->clock, 3, w->Heroes[d1].id, w->Bases[d2].id) ;
    printf("%d: CHEGA  HEROI %d BASE %d (%d/%d) DESISTE\n", w->clock,
           w->Heroes[d1].id, w->Bases[d2].id, w->Bases[d2].NHeroes,
           w->Bases[d2].size) ;
    insere_lef(w->lef, ev) ;
  }
  
  return 1 ;
}

int trata_evento_espera(struct world_t *w, struct hero_t *h, struct base_t *b) {

  if (!h || !b) 
    return 0 ;

  lista_insere(b->wait, h->id, -1) ;
  cria_evento(w->clock, 4, b->id, 0) ;
  printf("%d: ESPERA HEROI %d BASE %d (%d)\n", w->clock, h->id, b->id,
         lista_tamanho(b->wait)) ; 

  return 1 ;
}

void imprime_t (struct world_t *w) { //FUNCAO PARA TESTE

  int i ;
  char n ;

  n = 'W' ;

  printf("HEROIS\n") ;
  for (i = 0; i < N_HEROIS; i++) {
    
    printf("id %d, pac %d, spe %d, Bid %d ", w->Heroes[i].id,
            w->Heroes[i].patience, w->Heroes[i].speed, w->Heroes[i].BaseId) ;
    set_print(w->Heroes[i].Skills) ;
    printf("\n") ;
  }

  printf("BASES:\n") ;
  for (i = 0; i < N_BASES; i++) {
    
    printf("id %d, size %d, cx %d, cy %d ", w->Bases[i].id,
          w->Bases[i].size, w->Bases[i].cx, w->Bases[i].cy) ; 

    set_print(w->Bases[i].party) ;
    set_print(w->Bases[i].skills) ;
    lista_imprime(&n, w->Bases[i].wait) ;
  }

  /*printf("Missoes: \n") ;
  for (i = 0; i < N_MISSOES; i++) {
    
    printf("id %d, cx %d, cy %d ", w->Miss[i].id, w->Miss[i].cx,w->Miss[i].cy) ;
    set_print(w->Miss[i].skills) ;
    printf("\n") ;
  } */
}

// programa principal
int main () {

  struct world_t *w ;
  struct evento_t *ev ;
  /*struct hero_t *h ;
  struct base_t *b ;*/

  // iniciar o mundo
  srand(0) ;

  w = world_create(T_INICIO, N_TAMANHO_MUNDO, N_HABILIDADES, N_HEROIS,
                   N_BASES, N_MISSOES) ;
  world_start(w, T_FIM_DO_MUNDO) ;

  //imprime_lef(w->lef) ;
  imprime_t(w) ;

  // executar o laço de simulação
  while (w->clock < T_FIM_DO_MUNDO) {

    ev = retira_lef(w->lef) ; 
    w->clock = ev->tempo ;

    switch (ev->tipo) {

      case EV_CHEGA :
       
        trata_evento_chega(w, ev) ;
        destroi_evento(ev) ;
        break ;

      case EV_ESPERA :

        destroi_evento(ev) ;
        break ;

      case EV_DESISTE :

        destroi_evento(ev) ;
        break ;

      case EV_AVISA :

        destroi_evento(ev) ;
        break ;

      case EV_ENTRA :

        destroi_evento(ev) ;
        break ;

      case EV_SAI :

        destroi_evento(ev) ;
        break ;

      case EV_VIAJA :

        destroi_evento(ev) ;
        break ;

      case EV_MISSAO :

        destroi_evento(ev) ;
        break ;

      case EV_FIM :
        
        trata_evento_fim(w, ev) ;
        destroi_evento(ev) ;
        break ;

    }


  } 

  // destruir o mundo
  w = world_destroy(w) ;

  return 0 ;
}

