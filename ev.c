// programa da Biblioteca de Eventos "The Boys - 2023"
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
#include "ev.h"

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

int trata_evento_fim (struct world_t *w, struct evento_t *end) {

  int i ;
  float med, tent ;

  if (!w || !w->Heroes || !end) 
    return 0 ;

  printf("%d: FIM\n", end->tempo) ;

  for (i = 0 ; i < w->NHeroes ; i++) {

    printf("HEROI %2d PAC %3d VEL %4d EXP %3d HABS ", w->Heroes[i].id, 
            w->Heroes[i].patience, w->Heroes[i].speed, w->Heroes[i].xp) ;
  
    set_print(w->Heroes[i].Skills) ;
    printf("\n") ;
  }  

  /*calcula as medias de tentativas e de missoes cumpridas */
  med = (float) w->cont / w->NMiss * 100 ;
  tent = (float) w->tr / w->NMiss ;

  printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",
         w->cont, w->NMiss, med, tent ) ;

  return 1 ;
} 

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_chega(struct world_t *w, struct evento_t *ch) {

  struct evento_t *ev ;
  bool espera ;
  int d1, d2 ;

  if (!w || !w->lef || !w->Bases[ch->dado2].wait || !ch) 
    return 0 ; 

  d1 = ch->dado1 ;
  d2 = ch->dado2 ;
 
  /*define se o heroi espera ou nao*/
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
    insere_lef(w->lef, ev) ;
  }
  else {
  
    ev = cria_evento(w->clock, EV_DESISTE, w->Heroes[d1].id, w->Bases[d2].id) ;
    printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", w->clock,
           w->Heroes[d1].id, w->Bases[d2].id, set_card(w->Bases[d2].party),
           w->Bases[d2].size) ;
    insere_lef(w->lef, ev) ;
  }

  return 1 ;
}

/*retorna 1 em sucesso e 0 em caso de falha*/
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

/*retorna 1 em sucesso e 0 em caso de falha*/
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

/*retorna 1 em sucesso e 0 em caso de falha*/
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

  /*libera herois para entrar na base até sua capacidade max e
    une-os ao conj de habilidades da base*/
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

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_entra (struct world_t *w, struct hero_t *h, struct base_t *b) {

  int tpb, r, t ;
  struct evento_t *ev ;

  if (!w || !h || !b) 
    return 0 ;

  r = aleat(1, 20) ;
  tpb = 15 + (h->patience * r) ;

  printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d", w->clock, h->id,
         b->id, set_card(b->party), b->size, tpb + w->clock ) ;
  printf("\n") ;

  t = w->clock +tpb ;
  ev = cria_evento(t, EV_SAI, h->id, b->id) ;
  insere_lef(w->lef, ev) ;

  return 1 ;
}

/*retorna 1 em sucesso e 0 em caso de falha*/
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

  /*refaz o conj habilidades da base*/
  while (set_card(b->party) > i ) 
    for (j = j ; j < w->NHeroes; j++) 
      if (set_in(b->party, j)) {

        set_union(b->skills, w->Heroes[j].Skills, b->skills) ;
        i++ ;
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

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_viaja (struct world_t *w, struct hero_t *h, struct base_t *b) {

  int ca, co, dur, dis, t ;
  struct evento_t *ev ;

  if (!w || !h || !b)
    return 0 ;

  ca = (b->cx - w->Bases[h->BaseId].cx ) * (b->cx - w->Bases[h->BaseId].cx) ;
  co = (b->cy - w->Bases[h->BaseId].cy ) * (b->cy - w->Bases[h->BaseId].cy) ;

  dis = sqrt(ca + co) ;
  dur = dis / h->speed ;
  t = w->clock + dur ;

  ev = cria_evento( t, EV_CHEGA, h->id, b->id) ;
  insere_lef(w->lef, ev) ;
  printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
         w->clock, h->id, h->BaseId, b->id, dis, h->speed, w->clock + dur) ;

  return 1 ;
}

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_missao (struct world_t *w, struct evento_t *ev) {

  int i, j, bmp ;
  struct evento_t *ad ;
  struct miss_t *m ;
  struct base_t *b ;
  struct dist_t *v, aux ;

  if (!w || !ev) 
    return 0 ;

  /*alocando um vetor de prioridade*/
  v = malloc (sizeof(struct dist_t) * w->NBases) ; 
  if (!v)
    return 0 ;

  m = &w->Miss[ev->dado1] ;
  bmp = -1 ;

  /*inicializando o vetor*/
  for ( i = 0 ; i < w->NBases ; i++) {

    v[i].id = i ; 
    b = &w->Bases[v[i].id] ;
    v[i].d = sqrt(pow(m->cx - b->cx, 2) + pow(m->cy - b->cy, 2)) ;
  }

  printf("%6d: MISSAO %4d HAB REQ:", w->clock, m->id) ;
  set_print(m->skills) ;
  printf("\n") ;

  /*Ordena o vetor de bases por distancia da missao*/
  for (i = 0 ; i < w->NBases -1 ; i++) 
    for (j = i ; j < w->NBases ; j++) 
      if (v[i].d > v[j].d) {

        aux = v[i] ;
        v[i] = v[j] ;
        v[j] = aux ;
      }

  /*testa se as bases contem o set de habilidades da missao */
  for (i = 0 ; i < w->NBases ; i++) {
  
    b = &w->Bases[v[i].id] ;
    printf("%6d: MISSAO %4d HAB BASE %d:", w->clock, m->id, b->id) ;
    set_print(b->skills) ;
    printf("\n") ;

    if (set_contains(b->skills, m->skills)) {

      bmp = v[i].id ;
      break ;
    }
  }

  /*caso de missao impossivel*/
  if (bmp == -1) {

    ad = cria_evento(ev->tempo + 24 * 60, EV_MISSAO, ev->dado1, 0) ;
    insere_lef(w->lef, ad) ;
    printf("%6d: MISSAO %4d IMPOSSIVEL\n", w->clock, m->id);
    w->tr++ ;
    free(v) ;

    return 1 ;
  }

  /*concede xp aos herois*/ 
  for ( i = 0 ; i < w->NHeroes ; i++) 
    if ( set_in(b->party, i)) 
      w->Heroes[i].xp++ ;
    
  printf("%6d: MISSAO %4d CUMPRIDA BASE %d HEROIS:", w->clock, m->id, b->id) ; 
  set_print(b->party) ;
  printf("\n") ;
  w->cont++ ;
  
  free(v) ;

  return 1 ;
}

