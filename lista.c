// TAD lista de números inteiros - implementação
// Autor: Alexandre Sbrissia

// Implementação com lista encadeada dupla não-circular
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

//recebe uma lista e uma posição e retorna o endereço do node 
//funcao exclusiva desse arquivo
lnode_t * acha_node ( lista_t * lst, int pos) {

  int i ;
  lnode_t *cont ;

  cont = lst->head ;

  for( i = 0; i < pos && i < lst->size -1; i++)  //cont acha o node desejado
    cont = cont->next ;

  return (cont) ;
}

lista_t* lista_cria () {

  lista_t *ptr ;

  ptr = malloc( sizeof (lista_t)) ;

  ptr->size = 0 ;
  ptr->head = NULL ;
  ptr->tail = NULL ; 

  return ptr ;
}

lista_t* lista_destroi (lista_t* lst) {

  lnode_t *aux ;

  if ( lista_vazia( lst) == 1) {

    free( lst) ;
    lst = NULL ;

    return lst ;
  }

  while (lst->head) {

    aux = lst->head ; 
   
    lst->head = lst->head->next ;
    free( aux) ;
  }
  
  free( lst) ;

  return NULL ;
}

int lista_tamanho (lista_t* lst) { 

  return (lst->size) ;
}

int lista_vazia (lista_t* lst) {

  if(!lst)
    return -1 ;

  if( lst->size == 0)
    return 1 ;

  return 0 ;
}

int lista_procura (lista_t* lst, int elem) {

  int cont ;
  lnode_t *aux ;

  aux = lst->head ;

  cont = 0 ;

  while( aux->val != elem && aux != lst->tail) { //varre a lista até o valor apontado ser igual a elem

    aux = aux->next ;
    cont++ ;
  }

  if( aux->val == elem && aux == lst->tail) //caso elem esteja por ultimo
    return lst->size -1 ;

  if( aux->val != elem) //nao achou o elemento
    return -1 ;

  return (cont) ;

}

int lista_consulta (lista_t* lst, int* elem, int pos) {

  lnode_t *aux ;

  if(!elem || !lst)
    return -1 ;

  if ( pos < 0 || pos > lst->size -1)
    return -1 ;
  
  aux = acha_node(lst, pos) ; //aux recebe o endereço do node na posição desejada

  *elem = aux->val ;

  return lista_tamanho(lst) ;
}

int lista_insere (lista_t* lst, int elem, int pos) {

  int i ;
  lnode_t *aux, *cont ;

  if( !lst) 
    return -1 ;

  aux = malloc( sizeof( lnode_t)) ;

  aux->val = elem ;
  aux->prev = NULL ;
  aux->next = NULL ;

  if (lista_vazia( lst) == 1) {

    lst->head = aux ;
    lst->tail = aux ;
    lst->size++ ;

    return (lista_tamanho( lst)) ;
  }

  if ( pos == -1 || pos >= lst->size -1 ) { //casos que insere o node no fim

    aux->prev = lst->tail ;
    lst->tail->next = aux ;
    aux->next = NULL ;
    lst->tail = aux ;
    lst->size++ ;

    return( lista_tamanho(lst)) ;
  }

  if ( pos == 0) { //caso o numero inserido se torne o head

    lst->head->prev = aux ;
    aux->next = lst->head ;
    aux->prev = NULL ;
    lst->head = aux ;
    lst->size++ ;

    return( lista_tamanho(lst)) ;
  }

  //insere dentro da lista
  cont = lst->head ;

  for( i = 0; i < pos; i++)  
    cont = cont->next ;  

  aux->next = cont ;
  aux->prev = cont->prev ;
  cont->prev->next = aux ;
  cont->prev = aux ;
  lst->size++ ;

  return( lista_tamanho(lst)) ;
}

int lista_retira (lista_t* lst, int* elem, int pos) {

  lnode_t *aux ;

  if (!lst )
    return -1 ;

  if ( pos == 0 && lst->size == 1) { //caso onde a lista tem so um elemento

    aux = lst->head ;
    lst->tail = NULL ;
    lst->head = NULL ;
    lst->size-- ;
    *elem = aux->val ;
    free(aux) ;

    return (lista_tamanho( lst)) ;
  }

  if ( pos == 0 && lst->size > 1 ) { //caso onde head é alterado

    aux = lst->head ;
    lst->head = aux->next ;
    lst->head->prev = NULL ;
    aux->next = NULL ;
    lst->size-- ;
    *elem = aux->val ;
    free(aux) ;

    return( lista_tamanho( lst)) ;
  }


  if ( pos == lst->size -1 && lst->size > 1) { //caso onde o elemento a ser tirado é o ultimo

    aux = lst->tail ;
    lst->tail = aux->prev ;
    lst->tail->next = NULL ;
    aux->prev = NULL ;
    *elem = aux->val ;
    lst->size-- ;
    free(aux) ;

    return( lista_tamanho( lst)) ;
  }

  if ( pos > 0 && pos < lst->size -1) { //inserção entre head e tail

    aux = acha_node( lst, pos) ;

    //desconecta o node a ser retirado e conecta o antecessor ao sucessor
    aux->next->prev = aux->prev ;
    aux->prev->next = aux->next ;
    aux->prev = NULL ;
    aux->next = NULL ;

    lst->size-- ;
    *elem = aux->val ;
    free(aux) ;

    return( lista_tamanho( lst)) ;
  }

  return -1; //caso a posição não esteja na lista

}

void lista_imprime (char *nome, lista_t* lst) {

  lnode_t *ptr;
  if (!nome || !lst)
    return ;

  printf("%c: ", *nome) ; //imprime antes para servir nos dois casos

  if ( lista_vazia(lst) == 1 ) { //caso a lista esteja vazia, não imprime nenhum valor

    printf("[ ") ;
    printf("]") ;
    printf(" (0 elementos)\n") ;
    return ;
  }

  //demais casos
  ptr = lst->head ;

  printf("[ ") ;
  printf("%d ", ptr->val) ;

  while (lst->tail != ptr  ) {

    ptr = ptr->next ;
    printf( "%d ", ptr->val) ;
  }

  printf("]") ;
}



