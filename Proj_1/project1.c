/* 
 * iaed-23 - ist1106930 - project1
 * Construção de um sistema de gestão de carreiras de transportes públicos
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NOME_PARAGEM 51 /* máximo de caracteres do nome da paragem */
#define MAX_NOME_CARR 21    /* máximo de caracteres do nome da carreira */
#define MAX_PARAGENS 10000  /* número máximo de paragens */
#define MAX_CARREIRAS 200   /* número máximo de carreiras */
#define MAX_LIGACOES 30000  /* número máximo de ligações */

#define invLen 8  /* tamanho da string 'inverso' */
#define NAO_EXISTE -1		/* código de erro */
#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* -- Tipos de Dados -- */

typedef struct {
    char nome[MAX_NOME_PARAGEM];
    double lat;
    double lon;
} Paragem;

typedef struct {
    char nome[MAX_NOME_CARR];
    Paragem paragemInicial;
    Paragem paragemFinal;
} Carreira;

typedef struct {
    Carreira carreira;
    Paragem origem;
    Paragem destino;
    double custo;
    double duracao;
} Ligacao;

/* -- Variaveis Globais -- */

Paragem paragens[MAX_PARAGENS]; /* vetor de paragens */
int num_paragens = 0; /* número de paragens introduzidas */

Carreira carreiras[MAX_CARREIRAS]; /* vetor de carreiras */
int num_carreiras = 0; /* número de carreiras introduzidas */

Ligacao ligacoes[MAX_LIGACOES]; /* vetor de ligações */
int num_ligacoes = 0; /* número de ligações introduzidas */

/* vetor do nº de carreiras que passam por cada paragem*/
int numCarreirasParagem[MAX_PARAGENS]; 

/* vetor do custo total de cada paragem*/
double custoTotalCarreira[MAX_CARREIRAS];

/* vetor da duracao total de cada carreira*/
double duracaoTotalCarreira[MAX_CARREIRAS];

/* vetor do nº de paragens de cada carreira */
int numParagensCarreira[MAX_CARREIRAS];

/* vetor de carreiras por paragem para verificar as interseções */
int CarreirasPorParagem[MAX_PARAGENS][MAX_CARREIRAS] = {0}; 

/* -- Funcoes auxiliares -- */

/* Função que remove os dois primeiros caracteres do input */
void remover2CharsIniciais(char *str){
    int i, len;
    len = strlen(str);
    for (i = 2; i < len; i++) {
        str[i-2] = str[i];
        }
    str[i-2] = '\0';
}

/* Função que verifica se existe carreira */
int existeCarreira(char nome[]){
    int i;
    for (i = 0; i < num_carreiras; i++) {
        if (strcmp(carreiras[i].nome, nome) == 0) 
           return TRUE; 
    }
    return FALSE;
} 

/* Função que verifica se existe paragem */
int existeParagem(char nome[]){
    int i;
    for (i = 0; i < num_paragens; i++) {
        if (strcmp(paragens[i].nome, nome) == 0) 
           return TRUE; 
    }
    return FALSE;
}

/* Função que cria uma carreira */
void criarNovaCarreira(char nome[]) {
    Carreira nova_carreira;
    strcpy(nova_carreira.nome, nome);
    strcpy(nova_carreira.paragemInicial.nome, "");
    strcpy(nova_carreira.paragemFinal.nome, "");
    carreiras[num_carreiras] = nova_carreira;
    custoTotalCarreira[num_carreiras] = 0;
    duracaoTotalCarreira[num_carreiras] = 0;
    numParagensCarreira[num_carreiras] = 0;
    num_carreiras++;
}

/* Função que verifica se o utilizador escreveu a string inverso
ou uma das suas abreviaturas */
int inversoCheck(char inverso[]){
    int len = strlen(inverso);
    if (len > 2 && len < 8 && strncmp(inverso, "inverso", len) == 0) {
        return TRUE;
    }
    return FALSE;
}

/* Função que verifica se as paragens recebidas como argumento são os extremos
da carreira. */
int paragensExtremosCarreira(char carreira[], char paragemOrigem[], 
char paragemDestino[]){
    
    int i, j;

    for (i = 0; i < num_ligacoes; i++ ){
        if (strcmp(ligacoes[i].carreira.nome, carreira) == 0){
            for (j = 0; j < num_carreiras; j++){
                if (strcmp(carreiras[j].nome, carreira) == 0) {
                    if ( (strcmp(carreiras[j].paragemInicial.nome,
                    paragemDestino) != 0 && strcmp(carreiras[j].paragemFinal.
                    nome, paragemOrigem) != 0))
                        return FALSE;
                }
            }
        }
    }
    return TRUE;
}

/* Função que atualiza as paragens de extremidade de uma carreira de acordo com
as paragens de origem e destino recebidas como argumento. */
void atualizarParagemCarreira(char carreira[], char paragemOrigem[], char
paragemDestino[]) {
    
    int i, j, k;

    for (j = 0; j < num_ligacoes; j++ ){
        if (strcmp(ligacoes[j].carreira.nome, carreira) == 0){
            for (i = 0; i < num_carreiras; i++) {
                if (strcmp(carreiras[i].nome, carreira) == 0) {
                    if((strcmp(carreiras[i].paragemInicial.nome,paragemDestino)
                    ==0) 
                    &&(strcmp(carreiras[i].paragemFinal.nome, paragemOrigem) ==
                    0)){
                        /* caso da carreira circular*/
                        strcpy(carreiras[i].paragemFinal.nome, carreiras[i].
                        paragemInicial.nome);
                        return;
                    }
                    else if(strcmp(carreiras[i].paragemInicial.nome,
                    paragemDestino)==0){
                        strcpy(carreiras[i].paragemInicial.nome,paragemOrigem);
                        return;
                    } 
                    else if (strcmp(carreiras[i].paragemFinal.nome,
                    paragemOrigem) == 0) {
                        strcpy(carreiras[i].paragemFinal.nome, paragemDestino);
                        return;
                    }
                }
            }
        }
    }
    /*caso em que a carreira ainda não tem paragens associadas*/
    for (k = 0; k < num_carreiras; k++) {
        if (strcmp(carreiras[k].nome, carreira) == 0){
            strcpy(carreiras[k].paragemInicial.nome, paragemOrigem);
            strcpy(carreiras[k].paragemFinal.nome, paragemDestino);
            return;
        }
    }
}

 /*Esta função recebe o nome de uma carreira e retorna uma string com o 
percurso completo dessa carreira. A string retornada é armazenada no parâmetro 
strfinal. */
void percursoCarreira(char carreira[], char strfinal[]){
    int i,j, encontrouParagemInicial = FALSE;
    char nomeParagemInicial[MAX_NOME_PARAGEM], nomeParagemFinal
    [MAX_NOME_PARAGEM], ultimoDestino[MAX_NOME_PARAGEM];

    for( j= 0; j < num_carreiras; j++){
        if (strcmp(carreiras[j].nome, carreira) == 0){
            strcpy(nomeParagemInicial, carreiras[j].paragemInicial.nome);
            strcpy(nomeParagemFinal, carreiras[j].paragemFinal.nome);
        }
    }
    for (i = 0; i < num_ligacoes; i++) {
        if (strcmp(ligacoes[i].carreira.nome, carreira) == 0) {
            if (encontrouParagemInicial == FALSE) {
                if (strcmp(ligacoes[i].origem.nome, nomeParagemInicial) == 0){
                    encontrouParagemInicial = TRUE;
                    /*Adicionar a paragem inicial e a seguinte à strfinal */
                    sprintf(strfinal, "%s, ", ligacoes[i].origem.nome);
                    sprintf(strfinal + strlen(strfinal), "%s, ", ligacoes[i].
                    destino.nome);
                    /* Armazenar o nome da última paragem adicionada */
                    strcpy(ultimoDestino, ligacoes[i].destino.nome);
                    if (encontrouParagemInicial)
                        i = -1; 
                }
            } 
            else {
                /*Adicionar as paragens seguintes até chegar à paragem final */
                if ((strcmp(ligacoes[i].origem.nome, ultimoDestino) == 0) &&
                strcmp(ligacoes[i].origem.nome, nomeParagemInicial) != 0) {
                    sprintf(strfinal + strlen(strfinal), "%s, ", ligacoes[i].
                    destino.nome);
                    strcpy(ultimoDestino, ligacoes[i].destino.nome);
                    if (strcmp(ligacoes[i].destino.nome, nomeParagemFinal) ==
                    0) 
                        break;
                    else
                        i = -1;
                }
            }
        }
    }
    /* Remover a vírgula e o espaço finais da string de saída */
    if (strfinal[strlen(strfinal)-2] == ',') 
        strfinal[strlen(strfinal)-2] = '\0';
}

/*Esta função recebe o nome de uma carreira e retorna uma string com o percurso
completo dessa carreira por ordem inversa. A string retornada é armazenada no
parâmetro strfinal. */
void percursoCarreiraInverso(char carreira[], char strfinalInversa[]){
    int i,j, encontrouParagemInicial = FALSE;
    char nomeParagemInicial[MAX_NOME_PARAGEM], nomeParagemFinal
    [MAX_NOME_PARAGEM], ultimoDestino[MAX_NOME_PARAGEM];

    for( j= 0; j < num_carreiras; j++){
        if (strcmp(carreiras[j].nome, carreira) == 0){
            strcpy(nomeParagemInicial, carreiras[j].paragemInicial.nome);
            strcpy(nomeParagemFinal, carreiras[j].paragemFinal.nome);
        }
    }

    for (i = 0; i < num_ligacoes; i++) {
        if (strcmp(ligacoes[i].carreira.nome, carreira) == 0) {
            if (encontrouParagemInicial == 0) {
                if (strcmp(ligacoes[i].destino.nome, nomeParagemFinal) == 0) {
                    encontrouParagemInicial = TRUE;
                    /*Adicionar a paragem inicial e a seguinte à strfinal */
                    sprintf(strfinalInversa, "%s, ", ligacoes[i].destino.nome);
                    sprintf(strfinalInversa + strlen(strfinalInversa), "%s, ",
                    ligacoes[i].origem.nome);
                    /* Armazenar o nome da última paragem adicionada */
                    strcpy(ultimoDestino, ligacoes[i].origem.nome);
                    if (encontrouParagemInicial)
                        i = -1;
                }
            } 
            else {
                /*Adicionar as paragens seguintes até chegar à paragem final */
                if ((strcmp(ligacoes[i].destino.nome, ultimoDestino) == 0)&&
                    (strcmp(ligacoes[i].destino.nome, nomeParagemFinal) != 0)) {
                    sprintf(strfinalInversa + strlen(strfinalInversa), "%s, ",
                    ligacoes[i].origem.nome);
                    strcpy(ultimoDestino, ligacoes[i].origem.nome);
                    if (strcmp(ligacoes[i].origem.nome, nomeParagemInicial) ==
                    0)
                        break;
                    else
                        i = -1;
                }   
            }
        }
    }
    /* Remover a vírgula e o espaço finais da string de saída */
    if (strfinalInversa[strlen(strfinalInversa)-2] == ',') 
        strfinalInversa[strlen(strfinalInversa)-2] = '\0';
}

/* Função que atualiza o custo e a duração de uma carreira */
void updateCustoeDuracao(char carreira[], double custo, double duracao){
    int i;
    
    for(i = 0; i < num_carreiras; i++){
        if (strcmp(carreiras[i].nome, carreira) == 0){
            custoTotalCarreira[i] += custo;
            duracaoTotalCarreira[i] += duracao;
        } 
    }
}

/* Função que determina o número de paragens de uma carreira */
void numparagensCarreira(char carreira[]){
    int i, j, numParagens = 0;

    for (j = 0; j < num_ligacoes; j++) {
        if (strcmp(ligacoes[j].carreira.nome, carreira) == 0) 
            numParagens++;
    }
    for(i = 0; i < num_carreiras; i++){
        if (strcmp(carreiras[i].nome, carreira) == 0){
            if (numParagens == 1)
            /*caso onde a carreira tem apenas uma ligacao*/
                numParagensCarreira[i] = 2;
            else
                numParagensCarreira[i] = numParagens + 1;
            break;
        }
    }
}

/* Função que determina o número de carreiras que passam por uma paragem */
void numcarreirasParagem(char paragem[], char carreira[]){
    int i, paragem_index = NAO_EXISTE, carreira_index = NAO_EXISTE;

    /* Encontrar o indice da paragem */
    for (i = 0; i < num_paragens; i++) {
        if (strcmp(paragens[i].nome, paragem) == 0) {
            paragem_index = i;
            break;
        }
    }
    /* Encontrar o indice da carreira */
    for (i = 0; i < num_carreiras; i++) {
        if (strcmp(carreiras[i].nome, carreira) == 0) {
            carreira_index = i;
            break;
        }
    }
    /* Verificar se a carreira já passou por essa paragem */
    if (CarreirasPorParagem[paragem_index][carreira_index] != 1) {
        numCarreirasParagem[paragem_index]++;
        CarreirasPorParagem[paragem_index][carreira_index] = 1;
    }
}

/* Algoritmo bubblesort para ordenar as carreiras por ordem alfabética */
void ordenaCarreiras(char carreiras[MAX_CARREIRAS][MAX_NOME_CARR], int len) {
    int i, j;
    char temp[MAX_NOME_CARR];

    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - i - 1; j++) {
            if (strcmp(carreiras[j], carreiras[j+1]) > 0) {
                strcpy(temp, carreiras[j]);
                strcpy(carreiras[j], carreiras[j+1]);
                strcpy(carreiras[j+1], temp);
            }
        }
    }
}

/* -- Funcoes Principais -- */

/* Função que representa o comando 'p'-> adicionar e listar paragens */
void adiciona_e_lista_Paragens(char *input){ 
    char nome[MAX_NOME_PARAGEM] = "";
    double lat, lon;
    Paragem nova_paragem;
    
    /*comando para criar uma nova paragem*/
    if (sscanf(input, "%s %lf %lf", nome, &lat, &lon) == 3 ||
    sscanf(input, "\"%[^\"]\" %lf %lf", nome, &lat, &lon) == 3) {
        if (existeParagem(nome)){
            printf("%s: stop already exists.\n", nome);
            return;
        }
        /* criar paragem */
        strcpy(nova_paragem.nome, nome);
        nova_paragem.lat = lat;
        nova_paragem.lon = lon;
        numCarreirasParagem[num_paragens] = 0;
        paragens[num_paragens] = nova_paragem;
        num_paragens++;
    }
    /* comando para returnar as coordenadas de uma paragem */
    else if ((input[0] == '"' && sscanf(input+1, "%[^\"]", nome) == 1) ||
    sscanf(input, "%s", nome) == 1){
        int i, paragem_encontrada = FALSE;
        /* returnar as coordenadas da paragem */
        for (i = 0; i < num_paragens; i++) {
            if (strcmp(paragens[i].nome, nome) == 0) {
                printf("%16.12f %16.12f\n", paragens[i].lat, 
                paragens[i].lon);
                paragem_encontrada = TRUE;
                break;
            }
        }
        if (!paragem_encontrada) {
            printf("%s: no such stop.\n", nome);
            return;
        }
    }
    /* comando para listar todas as paragens existentes */  
    else if (strlen(input) == 0){
        /* listar todas as paragens */
        int i;
        for (i = 0; i < num_paragens; i++) {
            printf("%s: %16.12f %16.12f %d\n", paragens[i].nome, 
            paragens[i].lat, paragens[i].lon, 
            numCarreirasParagem[i]);
        }
    }
}

/* Função que representa o comando 'c'-> adicionar e listar carreiras */
void adiciona_e_lista_Carreiras(char *input){
    char nome[MAX_NOME_CARR], inverso[invLen] ="", 
    paragensCarreira[MAX_PARAGENS] = "";
    int i;

    if (sscanf(input, "%s %s", nome, inverso ) == 2){
        if (!existeCarreira(nome)){
            criarNovaCarreira(nome);
            return;
        }
        if (!inversoCheck(inverso)){
            printf("incorrect sort option.\n");
            return;
        }
        /*listar as paragens da carreira por ordem inversa*/
        percursoCarreiraInverso(nome, paragensCarreira);
        if (strlen(paragensCarreira))
            printf("%s\n", paragensCarreira);
        return;
    }
    if (sscanf(input, "%s", nome ) == 1){
        if (existeCarreira(nome)){
            /*listar as paragens da carreira*/
            percursoCarreira(nome, paragensCarreira);
            if (strlen(paragensCarreira))
                printf("%s\n", paragensCarreira);
            return;
        }
        criarNovaCarreira(nome);
        return;
    }
    else{
        /* listar todas as carreiras existentes*/
        for (i = 0; i < num_carreiras; i++) {
            if (carreiras[i].paragemInicial.nome[0] != '\0'){
                printf("%s %s %s %d %.2f %.2f\n", 
                carreiras[i].nome, carreiras[i].paragemInicial.nome, 
                carreiras[i].paragemFinal.nome, numParagensCarreira[i],
                custoTotalCarreira[i], duracaoTotalCarreira[i]);
            }
            else{ 
                /* caso onde a carreira ainda não tem paragens associadas */
                printf("%s %d %.2f %.2f\n", 
                carreiras[i].nome, numParagensCarreira[i] ,
                custoTotalCarreira[i], duracaoTotalCarreira[i]);
            }
        }
    }
}

/* Função que representa o comando 'l'-> adicionar ligações */
void adicionaLigacoes(char *input){
    char nomeCarreira[MAX_NOME_CARR], nomeOrigem[MAX_NOME_PARAGEM],   
    nomeDestino[MAX_NOME_PARAGEM];
    double custo, duracao;
    Ligacao nova_ligacao;

    if (sscanf(input, "%s %s %s %lf %lf", nomeCarreira, nomeOrigem,
    nomeDestino, &custo, &duracao) == 5 || 
    sscanf(input, "%s \"%[^\"]\" %s %lf %lf", nomeCarreira, nomeOrigem,
    nomeDestino, &custo, &duracao) == 5 ||
    sscanf(input, "%s %s \"%[^\"\n]\" %lf %lf", nomeCarreira, nomeOrigem,
    nomeDestino, &custo, &duracao) == 5 || 
    sscanf(input, "%s \"%[^\"]\" \"%[^\"\n]\" %lf %lf", nomeCarreira,
    nomeOrigem, nomeDestino, &custo, &duracao) == 5){
        if (!existeCarreira(nomeCarreira)){
            printf("%s: no such line.\n", nomeCarreira);
            return;
        }
        if (!existeParagem(nomeOrigem)){
            printf("%s: no such stop.\n", nomeOrigem);
            return;
        }
        if (!existeParagem(nomeDestino)){
            printf("%s: no such stop.\n", nomeDestino);
            return;
        }
        if (!paragensExtremosCarreira(nomeCarreira, nomeOrigem, nomeDestino)){
            printf("link cannot be associated with bus line.\n");
            return;
        }
        if (custo < 0 || duracao < 0){
            printf("negative cost or duration.\n");
            return;
        }
        /* criar a ligação */
        atualizarParagemCarreira(nomeCarreira, nomeOrigem, nomeDestino);
        strcpy(nova_ligacao.carreira.nome, nomeCarreira);
        strcpy(nova_ligacao.origem.nome, nomeOrigem);
        strcpy(nova_ligacao.destino.nome, nomeDestino);
        nova_ligacao.custo = custo;
        nova_ligacao.duracao = duracao;
        ligacoes[num_ligacoes] = nova_ligacao;
        num_ligacoes++; 
        updateCustoeDuracao(nomeCarreira, custo, duracao);
        numparagensCarreira(nomeCarreira);
        numcarreirasParagem(nomeOrigem, nomeCarreira);
        numcarreirasParagem(nomeDestino, nomeCarreira);
    }
}    

/* Função que representa o comando 'i'-> listar interseções entre carreiras */
void listaIntersecoes(){
    int i, j, l, carreiras = 0;
    char carreirasParagem[MAX_CARREIRAS][MAX_NOME_CARR];

    for (i = 0; i < num_paragens; i++){
        /* verifica se há mais do que 1 carreira a passar na paragem */
        if (numCarreirasParagem[i] > 1){
            for (j = 0; j < num_ligacoes; j++){
                /* encontrar as carreiras associada à paragem */
                if (strcmp(ligacoes[j].origem.nome, paragens[i].nome) == 0 ||
                strcmp(ligacoes[j].destino.nome, paragens[i].nome) == 0) {
                    /* verifica se a carreira em questão já está no vetor */
                    for (l = 0; l < carreiras; l++){
                        if(strcmp(carreirasParagem[l],
                        ligacoes[j].carreira.nome) == 0)
                            break;
                    }
                    /*se a carreira ainda não estiver no vetor, é adicionada*/
                    if (l == carreiras){
                        strcpy(carreirasParagem[carreiras], 
                        ligacoes[j].carreira.nome);
                        carreiras++;
                    }
                }
            }
            /* ordena a lista de carreiras em ordem alfabética */
            ordenaCarreiras(carreirasParagem, carreiras);

            /* imprime a lista de carreiras da paragem */
            printf("%s %d:", paragens[i].nome, carreiras);
            for(j = 0; j < carreiras; j++){
                printf(" %s", carreirasParagem[j]);
            }
            printf("\n");
            carreiras = 0;
            }
    }
}

/* -- main -- */

/* Lê o input e consoante o comando lido executa a função correta */
int main(){
    char input[100];

    while (fgets(input, 100, stdin) != NULL) {
        switch(input[0]) {
            case 'q':
                return 0;
            case 'p':
                remover2CharsIniciais(input);
                adiciona_e_lista_Paragens(input);
                break;
            case 'c':
                remover2CharsIniciais(input);
                adiciona_e_lista_Carreiras(input);
                break;
            case 'l':
                remover2CharsIniciais(input);
                adicionaLigacoes(input);
                break;
            case 'i':
                listaIntersecoes();
                break;
        }
    }
    return 0;
}
