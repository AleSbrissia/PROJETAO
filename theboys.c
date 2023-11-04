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
  int time ;
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

void random_skills (struct hero_t *h) {

  int i, tam ;

  if (!h)
    return ;

  for (i = 0; i < N_HEROIS; i++) {

    srand(i) ;

    h[i].Skills = set_create (N_HABILIDADES) ;
    tam = aleat(1, 3) ;

    //laço para evitar repetiçoes nas habilidades
    while( set_card(h[i].Skills) < tam)
      set_add (h[i].Skills, aleat(0, 9)) ;
    
  }
}

//funcao exclusiva deste arquivo
struct hero_t *Heroes_create () {

  int i , tam ;
  struct hero_t *h ;

  h = malloc(sizeof(struct hero_t) * N_HEROIS) ;
  if (!h)
    return NULL ;

  for (i = 0; i < N_HEROIS; i++) {

    srand(i) ;

    h[i].Skills = set_create (N_HABILIDADES) ;
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

    srand(i * 2) ;

    b[i].id = i ;
    b[i].size = aleat(3, 10) ;
    b[i].party = set_create(nheroes) ;
    b[i].cx = aleat(0, wsize -1) ;
    b[i].cy = aleat(0, wsize -1) ;
    b[i].wait = lista_cria() ;
  }
  return b ; 
}

// funcao exclusiva deste arquivo
// aloca um vetor de missoes e inicializa os conjuntos de habilidades
struct miss_t *Miss_create (int nmiss, int wsize) {

  int i ;
  struct miss_t *m ;

  m = malloc(sizeof(struct miss_t) * nmiss) ;
  if (!m)
    return NULL ;

  for (i = 0; i < nmiss; i++) {

    srand(i * 2) ;

    m[i].id = aleat(0, nmiss -1) ;
    m[i].cx = aleat(0, wsize -1) ;
    m[i].cy = aleat(0, wsize -1) ;
    m[i].skills = set_create(aleat(6, 10)) ;
  }
  return m ;
}

struct world_t *world_create () {

  struct world_t *w ; 

  w = malloc(sizeof(struct world_t)) ;

  w->NHeroes = N_HEROIS ;
  w->NBases = N_BASES ;
  w->NMiss = N_MISSOES ;
  w->WSize = N_TAMANHO_MUNDO ;
  w->clock = T_INICIO ;
  w->lef = cria_lef() ;

  //seta os vetores 
  w->Heroes = Heroes_create(N_HEROIS, N_HABILIDADES) ;
  w->Bases = Bases_create(N_TAMANHO_MUNDO, N_BASES, N_HEROIS) ;
  w->Miss = Miss_create(N_MISSOES, N_TAMANHO_MUNDO) ;

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

  for (i = 0; i < w->NMiss ; i++) 
    set_destroy (w->Miss[i].skills) ;
 
  destroi_lef(w->lef) ;
  free(w->Miss) ;
  free(w->Heroes) ;
  free(w->Bases) ;
  free(w) ;

  return NULL ;
}

 int world_start (struct world_t *w, int tend) {

  int i ;
  struct evento_t *ev ;

  if (!w || !w->Heroes || !w->Miss || !w->Bases) 
    return 0 ;

  for (i = 0; i < N_HEROIS; i++) {
    
    srand(i) ;

    w->Heroes[i].BaseId = aleat(0, N_BASES) ;
    w->Heroes[i].time = aleat(0, 4320) ;
    ev = cria_evento(w->clock, 1, w->Heroes[i].id, w->Heroes[i].BaseId) ;
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

//passar nos ponteiros o endereço deles no vetor
 int trata_evento_chega(struct world_t *w, struct hero_t *h, struct base_t *b) {

  struct evento_t *ev ;
  bool espera ;
  
  if (!w || !w->lef || !b || !h || !b->wait || !b->party || !h->Skills) 
    return 0 ; 
    
  if (lista_vazia(b->wait) && b->NHeroes < b->size) 
    espera = true ;
  else 
    espera = h->patience > (10 * lista_tamanho(b->wait)) ;
    
  if (espera) {
  
    printf("%d: CHEGA HEROI %d BASE %d (%d/%d) ESPERA\n", w->clock, h->id, b->id,
            b->NHeroes, b->size) ;
    ev = cria_evento(w->clock, 2, h->id, b->id) ;
    insere_lef(w->lef, ev) ;

    return 1 ;
  }

  printf("%d: CHEGA HEROI %d BASE %d (%d/%d) DESISTE\n", w->clock, h->id, b->id,
          b->NHeroes, b->size) ;
  ev = cria_evento(w->clock, 3, h->id, b->id) ;
  insere_lef(w->lef, ev) ;

  return 1 ;
} 

//passar nos ponteiros o endereço deles no vetor
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
            w->Heroes[i].patience,w->Heroes[i].speed, w->Heroes[i].BaseId) ;
    set_print(w->Heroes[i].Skills) ;
    printf("\n") ;
  }

  printf("BASES:\n") ;
  for (i = 0; i < N_BASES; i++) {
    
    printf("id %d, size %d, cx %d, cy %d ", w->Bases[i].id,
          w->Bases[i].size, w->Bases[i].cx, w->Bases[i].cy) ;

    set_print(w->Bases[i].party) ;
    lista_imprime(&n, w->Bases[i].wait) ;
  }

  /*for (i = 0; i < N_MISSOES; i++) {
    
    printf("Missoes: \n") ;
    printf("id %d, cx %d, cy %d ", w->Miss[i].id, w->Miss[i].cx,w->Miss[i].cy) ;
    set_print(w->Miss[i].skills) ;
    printf("\n") ;
  } */
}



// programa principal
int main () {

  struct world_t *w ;
  struct evento_t *ev ;
  //struct hero_t *h ;
  //struct base_t *b ;

  // iniciar o mundo

  w = world_create() ;

  world_start(w, T_FIM_DO_MUNDO) ;

  //imprime_t(w) ;

  imprime_lef(w->lef) ;
   

  // executar o laço de simulação
  while (w->clock < T_FIM_DO_MUNDO) {

    ev = retira_lef(w->lef) ; 

    w->clock = ev->tempo ;

    switch (ev->tipo) {

      case EV_CHEGA :
       
        /* h = w->Heroes[ev->dado1] ;
        b = w->Bases[ev->dado2] ;
        trata_evento_chega(w, &h, &b) ;*/
        printf("chega\n") ;
        free(ev) ; 
        break ;

      case EV_ESPERA :

        
        break ;

      case EV_DESISTE :

        break ;

      case EV_AVISA :

        break ;

      case EV_ENTRA :

        break ;

      case EV_SAI :

        break ;

      case EV_VIAJA :

        break ;

      case EV_MISSAO :

        break ;

      case EV_FIM :
        
        trata_evento_fim(w, ev) ;
        break ;

    }
  } 

  // destruir o mundo
  w = world_destroy(w) ;

  return (0) ;
}

