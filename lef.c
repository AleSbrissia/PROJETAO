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
struct evento_t *destroi_evento (struct evento_t *e) {

  if (!e)
    return NULL ;

  free(e) ;
  return NULL ;
}

/* 
 * Cria uma LEF vazia 
 * Retorna um ponteiro para a LEF ou NULL se falhar.
 */
struct lef_t *cria_lef () {
 
  struct lef_t *p ; 

  p = malloc(sizeof(struct lef_t)) ;

  if (!p) {
    printf("ERRO MALLOC CRIA_LEF \n") ;
    return NULL ;
  }

  p->primeiro = NULL ;

  return p ;
}
  
/* 
 * Destroi a LEF, liberando a memória ocupada por ela, inclusive os eventos.
 * Retorna NULL.
 */ 
struct lef_t *destroi_lef (struct lef_t *l) {

  struct nodo_lef_t *aux ;

  if (!l) 
    return NULL ;
  if (!l->primeiro) {
   
    free(l) ;
    return NULL ;
  }
 
  aux = l->primeiro ;
  /*destroi do começo para o fim*/
  while (aux->prox) {

    aux = l->primeiro ;
    if (!aux->prox) {
      
      free(aux->evento) ;
      free(aux) ;
      break ; 
    }

    l->primeiro = aux->prox ;
    free(aux->evento) ;
    free(aux) ;
  }
  free(l) ;

  return NULL ;
}

/*
 * Insere o evento apontado na LEF na ordem de tempos crescentes.
 * Eventos com o mesmo tempo devem respeitar a politica FIFO.
 * Retorna 1 em caso de sucesso ou 0 caso contrario.
*/
int insere_lef (struct lef_t *l, struct evento_t *e) {

	struct nodo_lef_t *aux, *p ;

	if (!l || !e)
		return 0 ;

	p = malloc (sizeof (struct nodo_lef_t)) ;
  if (!p)
    return 0 ;

	p->evento = e ;
	p->prox = NULL ;
	aux = l->primeiro ;

	if (aux == NULL) 
		l->primeiro = p ;

	else
    if (aux->evento->tempo > p->evento->tempo) {

      l->primeiro = p ;
      p->prox = aux ;
    }

	else 
    if (aux->prox == NULL) 
      aux->prox = p ;

 	else {

		while (aux->prox) {

			if (aux->prox->evento->tempo > p->evento->tempo) {

				p->prox = aux->prox;
				aux->prox = p;
				break;
			}
			aux = aux->prox ;
		} 
		aux->prox = p ;
	}

	return 1 ;
}


/* 
 * Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *retira_lef (struct lef_t *l) {

  struct evento_t *ev ;
  struct nodo_lef_t *pn ;
  
  if (!l)
    return NULL ; 

  if (vazia_lef(l))
    return NULL ;

  ev = l->primeiro->evento ;
  pn = l->primeiro ;
  pn->evento = NULL ;
  l->primeiro = l->primeiro->prox ;
  free(pn);

  return ev ;
}

/* 
 * Informa se a LEF está vazia.
 * Retorna 1 se estiver vazia, 0 se não estiver ou -1 se falhar.
 */
int vazia_lef (struct lef_t *l) {

  if (!l)
    return -1 ;

  if (!l->primeiro)
    return 1 ;

  return 0 ;
}

/* 
 * Imprime a LEF. Não faz parte do TAD, mas serve para depuração.
 * Formato de saída:
 * tempo tipo dado1 dado2
 * tempo tipo dado1 dado2
 * ...
 * total XX eventos
 */
void imprime_lef (struct lef_t *l) {

  struct nodo_lef_t *p ;
  int cont ;

  if (!l)
    return ;

  cont = 1 ;
  printf("LEF:\n") ;

  /*caso a lista esteja vazia*/
  if (vazia_lef(l)) {

    printf("Total 0 eventos\n") ;
    return ;
  }
  p = l->primeiro ;

  printf("tempo %d tipo %d dado1 %d dado2 %d\n", p->evento->tempo,
         p->evento->tipo, p->evento->dado1, p->evento->dado2) ;

  while (p && p->prox ) {

    p = p->prox ;
    printf("tempo %d tipo %d dado1 %d dado2 %d\n", p->evento->tempo,
           p->evento->tipo, p->evento->dado1, p->evento->dado2) ;

    cont++ ;
  }
  printf("Total %d eventos\n", cont) ;
}




