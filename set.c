#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"

// Creates an empty set capable of holding "size" values (from 0 to size -1)
// Return: pointer to the new set or NULL if failure
struct set_t *set_create (int max) {

  struct set_t *s ;
  int i ;

  s = malloc (sizeof (struct set_t)) ;
  
  if (!s)
    return NULL ;

  s->flag = malloc (max * sizeof (bool)) ;

  if (!s->flag) 
    return NULL ; 

  s->size = max ;
  s->num = 0 ;

  //set all the array elements at 0 
  for (i = 0 ; i < s->size ; i++) 
    s->flag[i] = 0 ;

  return s ;
}

// Destroys the set, freeing its memory
// Return: NULL
struct set_t *set_destroy (struct set_t *s) {

  if (!s || !s->flag)
    return NULL ;

  free(s->flag) ;
  free(s) ;

  return NULL ;
}

// Add an item to the set.
// Return: 1 if success or 0 if failure
int set_add (struct set_t *s, int item) {

  if (!s || !s->flag)
    return 0 ;

  if (item >= s->size) 
    return 0 ;
   
  s->flag[item] = 1 ;
  s->num++ ;

  return 1 ;
}

// Removes an item from the set.
// Return: 1 if success or 0 if failure
int set_del (struct set_t *s, int item) {

  if (!s || !s->flag) 
    return 0 ; 
  
  if (item >= s->size)
    return 0 ;

  if (s->flag[item] == 1) {

    s->flag[item] = 0 ;
    s->num-- ;
  }

  return 1 ;
}

// Verifies wether the item belongs to (is in) the set.
// Return: 1 if success or 0 if failure
int set_in (struct set_t *s, int item) { 

  if (!s || !s->flag) 
    return 0 ;
  if (s->flag[item])
    return 1 ;

  return 0 ;
}

// Verifies wether the set is empty
// Return: 1 if success or 0 if failure
int set_empty (struct set_t *s) { 

  if (!s || !s->flag) 
    return 0 ; 

  if (s->num == 0)
    return 1 ;

  return 0 ;
}

// Informs the set's cardinality (number of items or |S|)
// Return: n >=0 if success or -1 if failure
int set_card (struct set_t *s) {

  if (!s || !s->flag)
    return -1 ;

  return s->num ;
}

// Verifies wether s1 contains s2
// Return: 1 if success or 0 if failure
int set_contains (struct set_t *s1, struct set_t *s2) {

  int i ;
  bool t ;

  if (!s1 || !s1->flag || !s2 || !s2->flag)
    return 0 ;
  
  t = true ;
  for (i = 0 ; i < s2->size && t; i++) //varre o vetor procurando elementos presentes em s2 mas não em s1
    if (!s1->flag[i] && s2->flag[i]) 
      t = false ;

  if (!t) 
    return 0 ;

  return 1 ;
}

// Verifies wether s1 == s2
// Return: 1 if success or 0 if failure
int set_equal (struct set_t *s1, struct set_t *s2) {

  int i ;

  if (!s1 || !s2 || !s1->flag || !s2->flag)
    return 0 ;

  for (i = 0 ; i < s1->size && i < s2->size ; i++) //percorre o vetor procurando um elemento diferente
    if (s1->flag[i] != s2->flag[i])
      return 0 ;

  return 1 ;
}

// Puts in s3 the union of sets s1 and s2; s3 previous content
// is cleared, but its range (0, max) is not modified
// Return: 1 if success or 0 if failure
int set_union (struct set_t *s1, struct set_t *s2, struct set_t *s3) {

  int i ;

  if (!s1 || !s2 || !s3)
    return 0 ;
  if (!s1->flag || !s2->flag || !s3->flag)
    return 0 ;

  for (i = 0 ; i < s1->size || i < s2->size ; i++) 
    if (s1->flag[i] || s2->flag[i]) { 

      s3->flag[i] = 1 ;
      s3->num++ ;
    }

  return 1 ;
}

// Puts in s3 the intersection of sets s1 and s2; s3 previous content
// is cleared, but its range (0, max) is not modified
// Return: 1 if success or 0 if failure
int set_intersect (struct set_t *s1, struct set_t *s2, struct set_t *s3) {

  int i ;

  if (!s1 || !s2 || !s3)
    return 0 ;
  if (!s1->flag || !s2->flag || !s3->flag)
    return 0 ;

  for (i = 0 ; i < s1->size && i < s2->size ; i++) //percorre os vetores procurando elementos em comum
    if (s1->flag[i] && s2->flag[i]) { 

      s3->flag[i] = 1 ;
      s3->num++ ;
    }
  
  return 1 ;
}

// Puts in s3 the difference s1 - s2; s3 previous content is cleared,
// but its range (0, max) is not modified
// Return: 1 if success or 0 if failure
int set_diff (struct set_t *s1, struct set_t *s2, struct set_t *s3) {

  int i ;

  if (!s1 || !s2 || !s3)
    return 0 ;
  if (!s1->flag || !s2->flag || !s3->flag)
    return 0 ;
  
  for (i = 0 ; i < s1->size ; i++) //percorre o vetor selecionando os elementos nao subtraidos
    if (s1->flag[i] && !s2->flag[i]) { 
     
      s3->flag[i] = 1 ;
      s3->num++ ;
    }
    
  return 1 ;
}

// Puts in s2 a copy of set s1; s2 previous content is cleared,
// but its range (0, max) is not modified
// Return: 1 if success or 0 if failure
int set_copy (struct set_t *s1, struct set_t *s2) {

  int i ;
  
  if (!s1 || !s2)
    return 0 ;
  if (!s1->flag || !s2->flag)
    return 0 ;
  if (s1->size > s2->size)
    return 0 ;
  
  for (i = 0 ; i < s1->size ; i++) //percorre o vetor copiando 
    if (s1->flag[i]) {

      s2->flag[i] = 1 ;
      s2->num++ ;  
    }

  return 1 ;
}

// Prints the content of a set using this template:
// [ %d %d %d ] (%d items)\n
// (Does not belong to the TAD definition, but is useful for testing)
void set_print (struct set_t *s) {

  int i ;

  if (!s || !s->flag)
    return ;

  printf("[ ") ;
  for ( i = 0 ; i < s->size ; i++) 
    if (s->flag[i] == 1) 
      printf("%d ", i) ;
  printf("] ") ;
  
}




















