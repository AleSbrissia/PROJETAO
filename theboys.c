// programa principal do projeto "The Boys - 2023"
// Autor: Alexandre Sbrissia, GRR: 20231955

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "lista.h"
#include "libTB.h"

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

// programa principal
int main () {

  struct world_t *w ;
  struct evento_t *ev ;

  // iniciar o mundo
  w = world_create(T_INICIO, N_TAMANHO_MUNDO, N_HABILIDADES, N_HEROIS,
                   N_BASES, N_MISSOES) ;

  // executar o laço de simulação
  while (w->clock <= T_FIM_DO_MUNDO) {

    ev = retira_lef(w->lef) ; 
    w->clock = ev->tempo ;

    switch (ev->tipo) {

      EV_CHEGA :
        
        break ;

      EV_ESPERA :

        break ;

      EV_DESISTE :

        break ;

      EV_AVISA :

        break ;

      EV_ENTRA :

        break ;

      EV_SAI :

        break ;

      EV_VIAJA :

        break ;

      EV_MISSAO :

        break ;

      EV_FIM :
        
        trata_evento_fim ;
        break ;

    }


  }

  // destruir o mundo
  w = world_destroy(w) ;

  return (0) ;
}

