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

int trata_evento_fim (struct world_t *w, struct evento_t *end) {

  int i ;
  float med, tent ;

  if (!w || !w->Heroes) 
    return 0 ;

  printf("%d: FIM\n", end->tempo) ;

  for (i = 0 ; i < w->NHeroes ; i++) {

    printf("HEROI %2d PAC %3d VEL %4d EXP %3d HABS ", w->Heroes[i].id, 
            w->Heroes[i].patience, w->Heroes[i].speed, w->Heroes[i].xp) ;
  
    set_print(w->Heroes[i].Skills) ;
    printf("\n") ;
  }  

  med = (float) w->cont / w->NMiss * 100 ;
  tent = (float) w->tr / w->NMiss ;

  printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",
         w->cont, w->NMiss, med, tent ) ;

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
    
  if (lista_vazia(w->Bases[d2].wait) && set_card(w->Bases[d2].party) <
      w->Bases[d2].size) 
    espera = true ;
  else 
    espera = w->Heroes[d1].patience > (10 * lista_tamanho(w->Bases[d2].wait)) ;
    
  if (espera) {
  
    ev = cria_evento(w->clock , EV_ESPERA, w->Heroes[d1].id, w->Bases[d2].id) ;
    printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", w->clock,
           w->Heroes[d1].id, w->Bases[d2].id, set_card(w->Bases[d2].party),
           w->Bases[d2].size) ;
    w->clock++ ;
    insere_lef(w->lef, ev) ;
  }
  else {
  
    ev = cria_evento(w->clock +1, EV_DESISTE, w->Heroes[d1].id,
                       w->Bases[d2].id) ;
    printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", w->clock,
           w->Heroes[d1].id, w->Bases[d2].id, set_card(w->Bases[d2].party),
           w->Bases[d2].size) ;
    insere_lef(w->lef, ev) ;
  }
  return 1 ;
}

int trata_evento_espera(struct world_t *w, struct hero_t *h, struct base_t *b) {

  struct evento_t *ev ;

  if (!h || !b || !w) 
    return 0 ;

  lista_insere(b->wait, h->id, -1) ;
  ev = cria_evento(w->clock , EV_AVISA, 0, b->id) ;
  insere_lef(w->lef, ev) ;
  printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", w->clock, h->id, b->id,
         lista_tamanho(b->wait) -1) ; 

  return 1 ;
}

int trata_evento_desiste(struct world_t *w, struct evento_t *des) {

  int d1, d2, b ;
  struct evento_t *ev ;

  if (!w || !des) 
    return 0 ;

  d1 = des->dado1 ;
  d2 = des->dado2 ;
  b = aleat(0, w->NBases -1) ;

  ev = cria_evento(w->clock , EV_VIAJA, d1, b) ;
  insere_lef(w->lef, ev) ;
  printf("%6d: DESIST HEROI %2d BASE %d\n", w->clock, d1, d2) ;

  return 1 ;
}

int trata_evento_avisa(struct world_t *w, struct base_t *b) {
  
  int i ;
  struct evento_t *ev ;
  char n ;

  if (!w || !b) 
    return 0 ;

  n = ' ' ;

  printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA", w->clock, b->id, 
         set_card(b->party), b->size) ;
  lista_imprime(&n, b->wait) ;

  while (set_card(b->party) < b->size && !lista_vazia(b->wait)) {

    lista_retira(b->wait, &i, 0) ;
    set_add(b->party, i) ;
    set_union(b->skills, w->Heroes[i].Skills, b->skills) ;
    ev = cria_evento(w->clock, EV_ENTRA, i, b->id) ;
    insere_lef(w->lef, ev) ;

    printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", w->clock, b->id, i) ;
  }
  return 1 ;
}

int trata_evento_entra (struct world_t *w, struct hero_t *h, struct base_t *b) {

  int tpb, r ;
  struct evento_t *ev ;

  if (!w || !h || !b) 
    return 0 ;

  r = aleat(1, 20) ;
  tpb = 15 + (h->patience * r) ;
  printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d", w->clock, h->id,
         b->id, set_card(b->party), b->size, tpb + w->clock ) ;
  printf("\n") ;
  ev = cria_evento(w->clock +tpb, EV_SAI, h->id, b->id) ;
  insere_lef(w->lef, ev) ;

  return 1 ;
}

int trata_evento_sai (struct world_t *w, struct evento_t *sai) {

  int i, j, d1, d2 ;
  struct hero_t *h ; 
  struct base_t *b ;
  struct evento_t *ev ;

  if (!w || !sai) 
    return 0 ;

  h = &w->Heroes[sai->dado1] ;
  b = &w->Bases[sai->dado2] ;
  d1 = sai->dado1 ;
  d2 = sai->dado2 ;

  set_del(b->party, h->id) ;

  for (i = 0 ; i < w->NHeroes -1 ; i++) 
    set_del(b->skills, i) ;

  i = 0 ;
  j = 0 ;
  // laço para refazer o conj habilidades da base
  while (set_card(b->party) > i ) {
  
    for (j = j ; j < w->NHeroes; j++ )    {
     
      if (set_in(b->party, j)) {

        set_union(b->skills, w->Heroes[j].Skills, b->skills) ;
        i++ ;
      }
    }
  }

  ev = cria_evento(w->clock, EV_VIAJA, d1, aleat(0, w->NBases -1)) ;
  insere_lef(w->lef, ev) ;
  ev = cria_evento(w->clock, EV_AVISA, 0, d2) ; 
  insere_lef(w->lef, ev) ;
  printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)", w->clock, d1, d2, 
         set_card(w->Bases[d2].party), b->size) ;
  printf("\n") ;

  return 1 ;
}

int trata_evento_viaja (struct world_t *w, struct hero_t *h, struct base_t *b) {

  int ca, co, dur, dis ;
  struct evento_t *ev ;

  if (!w || !h || !b)
    return 0 ;

  ca = (b->cx - w->Bases[h->BaseId].cx ) * (b->cx - w->Bases[h->BaseId].cx) ;
  co = (b->cy - w->Bases[h->BaseId].cy ) * (b->cy - w->Bases[h->BaseId].cy) ;

  dis = sqrt(ca + co) ;
  dur = dis / h->speed ;

  ev = cria_evento(w->clock +dur, EV_CHEGA, h->id, b->id) ;
  insere_lef(w->lef, ev) ;
  printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
         w->clock, h->id, h->BaseId, b->id, dis, h->speed, w->clock + dur) ;

  return 1 ;
}

int trata_evento_missao (struct world_t *w, struct evento_t *ev) {

  int d1, d2, i, j, bmp, aux, *v ;
  struct evento_t *ad ;
  struct miss_t *m ;
  struct base_t *b1, *b2 ;

  if (!w || !ev) 
    return 0 ;

  v = malloc (sizeof(int) * w->NBases) ; //alocando um vetor de prioridade
  if (!v)
    return 0 ;

  for ( i = 0 ; i < w->NBases ; i++) 
    v[i] = i ; 

  m = &w->Miss[ev->dado1] ;
  //dis = 2 * N_TAMANHO_MUNDO ; //inicializa dis fora do mapa
  bmp = -1 ;
  
  printf("%6d: MISSAO %4d HAB REQ:", w->clock, m->id) ;
  set_print(m->skills) ;
  printf("\n") ;

  /* for (i = 0 ; i < w->NBases ; i++) {

    b1 = &w->Bases[v[i]] ;
    d1 = sqrt(pow(m->cx - b1->cx, 2) + pow(m->cy - b1->cy, 2)) ;
    printf("MISSAO b %d dis %d\n", i, d1) ;
  } */

  /*Ordena o vetor de bases por distancia da missao*/
  for (i = 0 ; i < w->NBases ; i++) {

    b1 = &w->Bases[v[i]] ;
    d1 = sqrt(pow(m->cx - b1->cx, 2) + pow(m->cy - b1->cy, 2)) ;
    
    for (j = 0 ; j < w->NBases ; j++) {

      b2 = &w->Bases[v[j]] ;
      d2 = sqrt(pow(m->cx - b2->cx, 2) + pow(m->cy - b2->cy, 2)) ;

      if (d2 > d1) {

        aux = v[i] ;
        v[i] = v[j] ;
        v[j] = aux ;
      }
    }
  }

  for (i = 0 ; i < w->NBases ; i++) {
  
    b1 = &w->Bases[v[i]] ;
    printf("%6d: MISSAO %4d HAB BASE %d:", w->clock, m->id, b1->id) ;
    set_print(b1->skills) ;
    printf("\n") ;

    if (set_contains(b1->skills, m->skills)) {

      bmp = i ;
      break ;
    }
  }

  if (bmp == -1) {

    ad = cria_evento(ev->tempo + 24 * 60, EV_MISSAO, ev->dado1, 0) ;
    insere_lef(w->lef, ad) ;
    printf("%6d: MISSAO %4d IMPOSSIVEL\n", w->clock, m->id);
    w->tr++ ;
    free(v) ;

    return 1 ;
  }

  for ( i = 0 ; i < w->NHeroes ; i++) 
    if ( set_in(b1->party, i)) 
      w->Heroes[i].xp++ ;
  
    
  printf("%6d: MISSAO %4d CUMPRIDA BASE %d HEROIS:", w->clock, m->id, b1->id) ; 
  set_print(b1->party) ;
  printf("\n") ;
  w->cont++ ;
  
  free(v) ;

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

