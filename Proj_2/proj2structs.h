/* 
 * iaed-23 - ist1106930 - project2 
 * Structs e defines usados no project2.c
*/

#ifndef PROJECT2_H
#define PROJECT2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALLOC_CARREIRAS 50 /* tamanho para reallocar o vetor carreiras */
#define MAXBUFFER 65535 /* máximo de caracteres do input */
#define NAO_EXISTE -1		/* código de erro */
#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* -- Tipos de Dados -- */

typedef struct Paragem {
    char* nome;
    double lat;
    double lon;
    int num_carreiras;
    char** carreiras;
    int maxcarreiras;
    struct Paragem* prox;
} Paragem;

typedef struct Ligacao{
    Paragem* origem;
    Paragem* destino;
    double custo;
    double duracao;
    struct Ligacao* prox;
    struct Ligacao* ant;
} Ligacao;

typedef struct Carreira{
    char* nome;
    double duracaoTotal;
    double custoTotal;
    int num_paragens;
    struct Ligacao* ligacoes;
    struct Ligacao* tail_ligacoes;
    struct Carreira* prox;
} Carreira;

#endif /* PROJECT2_H */