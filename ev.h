// Biblioteca de eventos do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

#ifndef _EV_H
#define _EV_H

#include "world.h"

struct dist_t {

  int id ;
  int d ;
} ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_fim (struct world_t *w, struct evento_t *end) ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_chega(struct world_t *w, struct evento_t *ch) ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_espera(struct world_t *w, struct hero_t *h, struct base_t *b) ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_desiste(struct world_t *w, struct evento_t *des) ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_avisa(struct world_t *w, struct base_t *b) ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_entra (struct world_t *w, struct hero_t *h, struct base_t *b) ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_sai (struct world_t *w, struct evento_t *sai) ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_viaja (struct world_t *w, struct hero_t *h, struct base_t *b) ;

/*retorna 1 em sucesso e 0 em caso de falha*/
int trata_evento_missao (struct world_t *w, struct evento_t *ev) ;

#endif
