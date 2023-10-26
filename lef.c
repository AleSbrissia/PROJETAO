/* disciplina de prog1, implementado por Alexandre Sbrissia GRR20231955 */

#include <stdio.h>
#include <stdlib.h>
#include "lef.h"

/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2) {

  struct evento_t *p ;

  p = malloc( sizeof( struct evento_t)) ;

  if (!p) 
    return NULL ;
 
  p->tempo = tempo ;
  p->tipo = tipo ;
  p->dado1 = dado1 ;
  p->dado2 = dado2 ;

  return p ;
}

/* 
 * Destroi um evento e retorna NULL.
 */ 
struct evento_t *destroi_evento (struct evento_t *e);

/* 
 * Cria uma LEF vazia 
 * Retorna um ponteiro para a LEF ou NULL se falhar.
 */
struct lef_t *cria_lef () {
 
  struct lef_t *p ; 

  p = malloc(sizeof(struct lef_t)) ;

  if (!p)
    return NULL ;

  p->primeiro = NULL ;

  return p ;
}
  
/* 
 * Destroi a LEF, liberando a memória ocupada por ela, inclusive os eventos.
 * Retorna NULL.
 */ 
struct lef_t *destroi_lef (struct lef_t *l);

/*
 * Insere o evento apontado na LEF na ordem de tempos crescentes.
 * Eventos com o mesmo tempo devem respeitar a politica FIFO.
 * Retorna 1 em caso de sucesso ou 0 caso contrario.
*/
int insere_lef (struct lef_t *l, struct evento_t *e);

/* 
 * Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *retira_lef (struct lef_t *l);

/* 
 * Informa se a LEF está vazia.
 * Retorna 1 se estiver vazia, 0 se não estiver ou -1 se falhar.
 */
int vazia_lef (struct lef_t *l);

/* 
 * Imprime a LEF. Não faz parte do TAD, mas serve para depuração.
 * Formato de saída:
 * tempo tipo dado1 dado2
 * tempo tipo dado1 dado2
 * ...
 * total XX eventos
 */
void imprime_lef (struct lef_t *l);

    







