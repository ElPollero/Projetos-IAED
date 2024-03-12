/* 
 * iaed-23 - ist1106930 - project2 
 * Construção de um sistema de gestão de carreiras de transportes públicos
 * Continuação do projeto 1
*/

#include "proj2structs.h"

/* Função para averiguar se a alocação de memória foi bem sucedida */
void checkMemory(void* pointer) {
    if (pointer == NULL) {
        printf("No memory.\n");
        exit(0);
    }
}

/* Funções para parsing do input */

/* Le espaços. Devolve 0 se chegou ao final de linha ou 1 caso contrário. */
int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c, stdin);
    return 1;
}

/* Le um nome para a string que recebe como parametro. */
void leNome(char** s) {
    int i = 0, c;
    checkMemory(*s);

    (*s)[0] = getchar();
    if ((*s)[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
            (*s)[i++] = c;
        ungetc(c, stdin);
    }
    else {
        while((c = getchar()) != '"')
            (*s)[i++] = c;
    }
    (*s)[i++] = '\0';

    *s = realloc(*s, i + 1);
    checkMemory(*s); 
}

/* Le todo o texto até ao final de linha. */
void leAteFinalLinha() {
    char c;
    while ((c = getchar()) != '\n') {
    }
}

/* -- Funcoes auxiliares -- */

/* --- Funções auxiliares para o comando 'p' --- */

/* Função que verifica se existe paragem */
int existeParagem(char nome[], Paragem *head){
    Paragem* current;
    if (head == NULL)
        return FALSE;

    current = head;
    while (current != NULL) {
        if (strcmp(current->nome, nome) == 0) {
            return TRUE;
        }
        current = current->prox;
    }
    return FALSE;
}

/* Função que cria uma paragem */
void criaParagem(char nome[], double lat, double lon, Paragem **head,
Paragem **tail){
    Paragem* nova_paragem = (Paragem*) malloc(sizeof(Paragem));
    checkMemory(nova_paragem);

    nova_paragem->nome = (char*) malloc(strlen(nome) + 1);
    nova_paragem->carreiras = (char**) malloc(ALLOC_CARREIRAS * sizeof(char*));
    checkMemory(nova_paragem->nome);
    checkMemory(nova_paragem->carreiras);

    strcpy(nova_paragem->nome, nome);
    nova_paragem->lat = lat;
    nova_paragem->lon = lon;
    nova_paragem->prox = NULL;
    nova_paragem->num_carreiras = 0;
    nova_paragem->maxcarreiras = ALLOC_CARREIRAS;

    if (*head == NULL) { 
        *head = nova_paragem;
        *tail = nova_paragem;
    } 
    else {
       (* tail)->prox = nova_paragem;
        *tail = nova_paragem;
    }
}

/* Função que encontra uma paragem */
Paragem* procuraParagem(char nome[], Paragem* head) {
    Paragem* current;
    if (head == NULL)
        return NULL;
    
    current = head;
    while (current != NULL) {
        if (strcmp(current->nome, nome) == 0) {
            return current;
        }
        current = current->prox;
    }
    return NULL;
}

/* Funçáo que lista todas as paragens existentes */
void listarParagens(Paragem* head) {
    Paragem* paragem = head;
    while (paragem != NULL) {
        printf("%s: %16.12f %16.12f %d\n", paragem->nome, paragem->lat, 
        paragem->lon, paragem->num_carreiras);
        paragem = paragem->prox;
    }
}

/* Função que vai dando update nas carreiras de cada paragem */
void updateCarreirasporParagem(char carreira[], char paragem[], Paragem* head){
    Paragem* current = procuraParagem(paragem, head);
    char** temp;
    int i;

    /* ver se a paragem já continha a carreira */
    for (i = 0; i < current->num_carreiras; i++) {
        if (strcmp(current->carreiras[i], carreira) == 0) 
                return;
    }

    current->num_carreiras++;
    if (current->num_carreiras == current->maxcarreiras){
        temp = (char**) realloc(current->carreiras, ALLOC_CARREIRAS +
        current->num_carreiras);
        checkMemory(temp);
        
        current->carreiras = temp;
        current->maxcarreiras += ALLOC_CARREIRAS;
    }

    current->carreiras[current->num_carreiras - 1] = malloc(strlen(carreira) + 
    1);
    checkMemory(current->carreiras[current->num_carreiras - 1]);
    strcpy(current->carreiras[current->num_carreiras - 1], carreira);
}

/* --- Funções auxiliares para o comando 'c' --- */

/* Função que verifica se existe carreira */
int existeCarreira(char nome[], Carreira* head){
    Carreira* current;
    if (head == NULL)
        return FALSE;

    current = head;
    while (current != NULL) {
        if (strcmp(current->nome, nome) == 0) {
            return TRUE;
        }
        current = current->prox;
    }
    return FALSE;
}

/* Função que cria uma Carreira */
void criaCarreira(char nome[], Carreira **head, Carreira **tail){
    Carreira* nova_carreira = (Carreira*) malloc(sizeof(Carreira));
    checkMemory(nova_carreira);

    nova_carreira->nome = (char*) malloc(strlen(nome) + 1);
    checkMemory(nova_carreira->nome);

    strcpy(nova_carreira->nome, nome);
    nova_carreira->num_paragens = 0;
    nova_carreira->duracaoTotal = 0;
    nova_carreira->custoTotal = 0;
    nova_carreira->prox = NULL;
    nova_carreira->ligacoes = NULL;
    nova_carreira->tail_ligacoes = NULL;

    if (*head == NULL) { 
        *head = nova_carreira;
        *tail = nova_carreira;
    } 
    else {
       (* tail)->prox = nova_carreira;
        *tail = nova_carreira;
    }
}

/* Função que encontra uma carreira */
Carreira* procuraCarreira(char nome[], Carreira* head){
    Carreira* current;

    if (head == NULL)
        return NULL;

    current = head;
    while(current != NULL ){
        if (strcmp(current->nome, nome) == 0)
            return current;
        current = current->prox;
    }
    return NULL;
}

/* Função que lista todas as carreiras */
void listaCarreiras(Carreira *head) {
    Carreira* carreira = head;
    while (carreira != NULL) {
        if (carreira->num_paragens != 0){
            printf("%s %s %s %d %.2f %.2f\n", 
            carreira->nome, carreira->ligacoes->origem->nome, 
            carreira->tail_ligacoes->destino->nome, carreira->num_paragens,
            carreira->custoTotal, carreira->duracaoTotal);
            carreira = carreira->prox;
        }
        else{
            printf("%s %d %.2f %.2f\n", carreira->nome,
            carreira->num_paragens, carreira->custoTotal,
            carreira->duracaoTotal);
            carreira = carreira->prox;
        }
    }
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

 /* Função que devolve o percurso de uma carreira por ordem */
void percursoCarreira(char nome[], Carreira* head){
    Carreira* carreira = procuraCarreira(nome, head);
    Ligacao* current = carreira->ligacoes;

    while(current != NULL){
        printf("%s, ", current->origem->nome);
        current = current->prox;
    }
    if (carreira->num_paragens != 0)
        printf("%s\n", carreira->tail_ligacoes->destino->nome);
}

/* Função que devolve o percurso de uma carreira no sentido inverso */
void percursoCarreiraInverso(char nome[], Carreira* head){
    Carreira* carreira = procuraCarreira(nome, head);
    Ligacao* current = carreira->tail_ligacoes;

    while(current != NULL){
        printf("%s, ", current->destino->nome);
        current = current->ant;
    }
    if (carreira->num_paragens != 0)
        printf("%s\n", carreira->ligacoes->origem->nome);
}

/* --- Funções auxiliares para o comando 'l' --- */

/* Le nome de carreira e paragens de uma ligacao. */
void leNomesComando(char** nomeCarreira, char** nomeOrigem,char** nomeDestino)
{
    leEspacos();
    leNome(nomeCarreira);
    leEspacos();
    leNome(nomeOrigem);
    leEspacos();
    leNome(nomeDestino);
}

/* Função que verifica se as paragens recebidas como argumento são os extremos
da carreira. */
int paragensExtremosCarreira(char carreira[], char paragemOrigem[], 
char paragemDestino[], Carreira* head){
        Carreira* current = head;

        while(current != NULL ){
            if (strcmp(current->nome, carreira) == 0)
                break;
            current = current->prox;
        }
        if (current->ligacoes == NULL)
            return TRUE;
        
        if (strcmp(current->ligacoes->origem->nome, paragemDestino) != 0 &&
        strcmp(current->tail_ligacoes->destino->nome, paragemOrigem) != 0)
            return FALSE;
        
        return TRUE;
}

/* Função que valida o input recebido no comando 'l' para o poder executar */
int validateInput(char* carreira, char* Porigem, char* Pdestino, double custo,
double duracao, Paragem** headP, Carreira** headC) {
    if (!existeCarreira(carreira, *headC)){
        printf("%s: no such line.\n", carreira);
        return FALSE;
    }
    if (!existeParagem(Porigem, *headP)){
        printf("%s: no such stop.\n",Porigem);
        return FALSE;
    }
    if (!existeParagem(Pdestino, *headP)){
        printf("%s: no such stop.\n",Pdestino);
        return FALSE;
    }
    if (!paragensExtremosCarreira(carreira,Porigem,Pdestino,*headC)){
        printf("link cannot be associated with bus line.\n");
        return FALSE;
    }
    if (custo < 0 || duracao < 0){
        printf("negative cost or duration.\n");
        return FALSE;
    }
    return TRUE;
}

/* Função que cria uma ligação */
Ligacao* criaLigacao(char carreira[], char paragemOrigem[], char
paragemDestino[], double custo, double duracao, Paragem* headP, Carreira* 
headC){
    Carreira* current = headC;
    Ligacao* nova_ligacao = (Ligacao*)malloc(sizeof(Ligacao));
    checkMemory(nova_ligacao);

    nova_ligacao->origem = procuraParagem(paragemOrigem, headP);
    nova_ligacao->destino = procuraParagem(paragemDestino, headP);
    nova_ligacao->custo = custo;
    nova_ligacao->duracao = duracao;
    nova_ligacao->prox = NULL;
    nova_ligacao->ant = NULL;

    while(current != NULL ){
        if (strcmp(current->nome, carreira) == 0)
            break;
        current = current->prox;
    }
    if (current->num_paragens == 0)
        current->num_paragens = 2;
    else
        current->num_paragens++;

    current->custoTotal += custo;
    current->duracaoTotal += duracao;

    return nova_ligacao;
}

/* Função que adiciona uma ligacao no fim da lista de ligacões de uma 
carreira*/
void acrescentaLigacaoFim(Carreira* carreira, Ligacao* novaLigacao){
    carreira->tail_ligacoes->prox = novaLigacao;
    novaLigacao->ant = carreira->tail_ligacoes;
    carreira->tail_ligacoes = novaLigacao;
}

/* Função que adiciona uma ligacao no início da lista de ligacões de uma 
carreira*/
void acrescentaLigacaoInicio(Carreira* carreira, Ligacao* novaLigacao){
    novaLigacao->prox = carreira->ligacoes;
     carreira->ligacoes->ant = novaLigacao;
    carreira->ligacoes = novaLigacao;
}

/* Adiciona primeira ligacao da carreira. */
void adicionaPrimeiraLigacao(Carreira* carreira, Ligacao* novaLigacao) {
    carreira->ligacoes = novaLigacao;
    carreira->tail_ligacoes = novaLigacao;
}

/* Funcao que adiciona uma ligacao à lista de ligações de uma certa carreira */
void adicionaLigacao(char carreira[], char paragemOrigem[], char
paragemDestino[], Carreira* headC, double custo, double duracao, Paragem* 
headP){
    Ligacao* nova_Ligacao = criaLigacao(carreira, paragemOrigem,
    paragemDestino, custo, duracao, headP, headC);
    Carreira* current = procuraCarreira(carreira, headC);

    if (current->ligacoes == NULL){
        adicionaPrimeiraLigacao(current, nova_Ligacao);
        return;
    }
    if (strcmp(current->ligacoes->origem->nome, paragemDestino) == 0 &&
    strcmp(current->tail_ligacoes->destino->nome, paragemOrigem) == 0){
        acrescentaLigacaoFim(current, nova_Ligacao);
        return;
    }
    if(strcmp(current->ligacoes->origem->nome, paragemDestino) == 0){
        acrescentaLigacaoInicio(current, nova_Ligacao);
        return;
    }
    if (strcmp(current->tail_ligacoes->destino->nome, paragemOrigem)== 0){
        acrescentaLigacaoFim(current, nova_Ligacao);
        return;
    }
}

/* --- Funções auxiliares para o comando 'i' --- */

/* Algoritmo bubblesort para ordenar as carreiras por ordem alfabética */
void ordenaCarreiras(char **carreiras, int len) {
    int i, j;
    char temp[MAXBUFFER];

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

/* --- Funções auxiliares para o comando 'r' --- */

/* Função que elimina uma carreira do vetor das carreiras de uma paragem */
void removeCarreirafromParagem(Paragem* paragem, Carreira* carreira){
    int i,j;
    
    for (i = 0; i < paragem->num_carreiras; i++) {
            if (strcmp(paragem->carreiras[i],carreira->nome) == 0) {
                free(paragem->carreiras[i]);
                for (j = i; j < paragem->num_carreiras - 1; j++) {
                    paragem->carreiras[j] = paragem->carreiras
                    [j + 1];
                }
                paragem->num_carreiras--;
                break;
            }
        }
}

/* Função que elimina uma carreira de todas as paragens pela qual ela passa 
e liberta as ligacoes da mesma carreia */
void removeCarreiraFromParagens(Carreira *carreira) {
    Ligacao *current_ligacao = carreira->ligacoes, *temp_ligacao = NULL;
    Paragem *current_paragem = NULL;

    while (current_ligacao != NULL) {
        current_paragem = current_ligacao->origem;
        /* remover carreira da paragem origem da ligação */
        removeCarreirafromParagem(current_paragem, carreira);
        /* avançar para o seguinte nó da lista de ligações */
        temp_ligacao = current_ligacao;
        current_ligacao = current_ligacao->prox;
        if (current_ligacao == NULL) {
            /* remover carreira da paragem destino da ligação */
            current_paragem = temp_ligacao->destino;
            removeCarreirafromParagem(current_paragem, carreira);
        }
        free(temp_ligacao);
    }
}

/* --- Funções auxiliares para o comando 'e' --- */

/* Função que dá reset à carreira */
void resetCarreira(Carreira* carreira){
    carreira->num_paragens = 0;
    carreira->custoTotal = 0;
    carreira->duracaoTotal = 0;
    free(carreira->ligacoes);
    carreira->ligacoes = NULL;
    carreira->tail_ligacoes = NULL;
}

/* Função que trata o caso onde a carreira tem apenas uma ligação */
void apenasUmaLigacao(Carreira* carreira, Ligacao* ligacao, Paragem* paragem){
    if (ligacao->origem == ligacao->destino) {
        resetCarreira(carreira);
        return;
    }
    else if (ligacao->origem == paragem) {
        removeCarreirafromParagem(ligacao->destino, carreira);
        resetCarreira(carreira);
        return;
    }
    else if (ligacao->destino == paragem) {
        removeCarreirafromParagem(ligacao->origem, carreira);
        resetCarreira(carreira);
        return;
    }
}

/*Função que trata do caso onde a paragem a eliminar é a primeira do percurso*/
void removeParagemInicio(Carreira* current_carreira, Ligacao**current_ligacao){
    Ligacao* temp_ligacao = NULL;

    current_carreira->ligacoes = (*current_ligacao)->prox;
    if ((*current_ligacao)->prox != NULL)
        (*current_ligacao)->prox->ant = NULL;
    current_carreira->custoTotal -= (*current_ligacao)->custo;
    current_carreira->duracaoTotal -= (*current_ligacao)->duracao;
    temp_ligacao = (*current_ligacao)->prox;
    free((*current_ligacao));
    (*current_ligacao) = temp_ligacao;
    current_carreira->num_paragens--;
}

/*Função que trata do caso onde a paragem a eliminar é a última do percurso*/
void removeParagemFim(Carreira *current_carreira, Ligacao **current_ligacao, 
Ligacao **prev_ligacao) {
    current_carreira->custoTotal -= (*current_ligacao)->custo;
    current_carreira->duracaoTotal -= (*current_ligacao)->duracao;
    free((*current_ligacao));
    (*current_ligacao) = NULL;
    (*prev_ligacao)->prox = NULL;
    current_carreira->tail_ligacoes = (*prev_ligacao);
    current_carreira->num_paragens--;
}

/* Função que dá update no nó da Ligação quando removemos a paragem do meio do
percurso */
void updateLigacaoNode(Carreira *current_carreira, Ligacao **current_ligacao){
    (*current_ligacao)->destino = (*current_ligacao)->prox->destino;
    (*current_ligacao)->custo += (*current_ligacao)->prox->custo;
    (*current_ligacao)->duracao += (*current_ligacao)->prox->duracao;
    current_carreira->num_paragens--;
}

/* Função que remove a paragem do meio do percurso quando ela não se repete */
void removeParagemMeio(Carreira *current_carreira, Ligacao **current_ligacao,
Ligacao **prev){
    Ligacao *temp_ligacao;

    updateLigacaoNode(current_carreira, current_ligacao);
    if ((*current_ligacao)->prox->prox == NULL) {
        current_carreira->tail_ligacoes = (*current_ligacao);
        free((*current_ligacao)->prox);
        (*current_ligacao)->prox = NULL;
        return;
    }
    else {
        (*current_ligacao)->prox->prox->ant = (*current_ligacao);
        temp_ligacao = (*current_ligacao)->prox->prox;
        free((*current_ligacao)->prox);
        (*current_ligacao)->prox = temp_ligacao;
        *prev = (*current_ligacao);
        (*current_ligacao) = temp_ligacao;
    }

}

/* Função que trata dos casos onde a paragem a ser removida encontra-se no
meio do percurso */
void mergeLigacoes(Carreira *current_carreira, Ligacao **current_ligacao,
Ligacao **prev) {
    int flag = 0;
    Ligacao *temp_ligacao = *prev;
    
/* caso em que a paragem a ser removida se encontra várias vezes de seguida */
    while ((*current_ligacao)->prox != NULL &&
    (*current_ligacao)->destino == (*current_ligacao)->prox->destino){
        flag = 1;
        updateLigacaoNode(current_carreira, current_ligacao);
        if ((*current_ligacao)->prox->prox == NULL)
            current_carreira->tail_ligacoes = (*current_ligacao);
        else    
            (*current_ligacao)->prox->prox->ant = (*current_ligacao);
        temp_ligacao = (*current_ligacao)->prox->prox;
        free((*current_ligacao)->prox);
        (*current_ligacao)->prox = temp_ligacao;
    }
/*caso em que a paragem a ser removida é diferente da paragem seguinte */
    if (flag == 0) {
        removeParagemMeio(current_carreira,current_ligacao,prev);
    }
}

/* Função que dá update na linked list das ligacoes cuja carreira contém a
paragem que foi removida pelo utilizador */
void updateLigacaoList(Paragem *current_paragem, Carreira *current_carreira, 
Ligacao *current_ligacao, Ligacao *prev_ligacao) {
    
    while (current_ligacao != NULL) {
        if (current_carreira->num_paragens == 2 && (current_ligacao->origem ==
        current_paragem || current_ligacao->destino == current_paragem)){
            /*carreira só tem uma ligação e está lá a paragem a ser removida */
            apenasUmaLigacao(current_carreira, current_ligacao,
            current_paragem);
            break;
        } 
        else if (current_ligacao->origem == current_paragem && prev_ligacao ==
        NULL) {
            /* paragem a ser removida encontra-se no início do percurso */
            removeParagemInicio(current_carreira, &current_ligacao);
        } 
        else if (current_ligacao->destino == current_paragem) {
            if (current_ligacao->prox == NULL) {
                /* paragem a ser removida encontra-se no final do percurso */
                removeParagemFim(current_carreira, &current_ligacao,
                &prev_ligacao);
                break;
            } 
            else {
                /* paragem a ser removida encontra-se no meio do percurso */
                mergeLigacoes(current_carreira,&current_ligacao,&prev_ligacao);
            }
        } 
        else {
            /* a paragem escolhida não se encontra na current_ligacao */
            prev_ligacao = current_ligacao;
            current_ligacao = current_ligacao->prox;
        }
    }
}

/* Função que itera sobre o vetor de carreiras da paragem e atualiza a
linked listed de ligações de cada carreira */
void updateLigacaoListForCarreiras(Paragem* current_paragem, Carreira* headC){
    Ligacao* current_ligacao;
    Carreira* current_carreira;
    Ligacao* prev_ligacao;
    int i;

    /* iterar sobre o vetor de carreiras da paragem */
    for (i = 0; i < current_paragem->num_carreiras; i++) {
        current_carreira =procuraCarreira(current_paragem->carreiras[i],headC);
        current_ligacao = current_carreira->ligacoes;
        prev_ligacao = NULL;
        updateLigacaoList(current_paragem, current_carreira, current_ligacao,
        prev_ligacao);
    }
}

/* Funcao que liberta a memoria ocupada pela Paragem nas linked lists */
void freeParagem(Paragem* paragem) {
    int i;
    for (i = 0; i < paragem->num_carreiras; i++) {
        free(paragem->carreiras[i]);
    }
    free(paragem->carreiras);
    free(paragem->nome);
    free(paragem);
}

/* --- Funções auxiliares para o comando 'a' --- */

/* Função que apaga a linked list das paragens */
void apagaParagensList(Paragem** headP, Paragem** tailP){
    Paragem *temp, *current_paragem = *headP;
    int i;

    while (current_paragem != NULL) {
        temp = current_paragem;
        current_paragem = current_paragem->prox;
        for (i = 0; i < temp->num_carreiras; i++) {
            free(temp->carreiras[i]);
        }
        free(temp->nome);
        free(temp->carreiras);
        free(temp);
    }
    *headP = NULL;
    *tailP = NULL;
}

/* Função que apaga a linked list das carreiras */
void apagaCarreirasList(Carreira** headC, Carreira** tailC){
    Carreira *temp, *current_carreira = *headC;
    Ligacao *tempL , *current_ligacao;

    while (current_carreira != NULL) {
        current_ligacao = current_carreira->ligacoes;
        while (current_ligacao != NULL) {
            tempL = current_ligacao;
            current_ligacao = current_ligacao->prox;
            free(tempL);
        }
        current_carreira->ligacoes = NULL;
        temp = current_carreira;
        current_carreira = current_carreira->prox;
        free(temp->nome);
        free(temp);
    }
    *headC = NULL;
    *tailC = NULL;
}

/* -- Funcoes Principais -- */

/* Função que representa o comando 'p'-> adicionar e listar paragens */
void adiciona_e_lista_Paragens(Paragem **head, Paragem ** tail){ 
    char *nome = malloc(sizeof(char) * MAXBUFFER);
    int fimLinha = leEspacos();

    if (!fimLinha){
        /* listar todas as paragens */
        listarParagens(*head);
        free(nome);
        return;
    }
    leNome(&nome);
    fimLinha = leEspacos();
    if(!fimLinha){
        /* comando para returnar as coordenadas de uma paragem */
        Paragem* paragem = procuraParagem(nome, *head);
        if (paragem == NULL)
            printf("%s: no such stop.\n", nome);
        else
            printf("%16.12f %16.12f\n", paragem->lat, paragem->lon);
    }
    else{
        /*comando para criar uma nova paragem*/
        double lat, lon;
        if (scanf("%lf%lf", &lat, &lon)  == 2){
            if (existeParagem(nome, *head))
                printf("%s: stop already exists.\n", nome);
            else
                criaParagem(nome, lat, lon, head, tail);
            leAteFinalLinha();
        }
    }
    free(nome);
}

/* Função que representa o comando 'c'-> adicionar e listar carreiras */
void adiciona_e_lista_Carreiras(Carreira **head, Carreira **tail){
    char *nome = malloc(sizeof(char) * MAXBUFFER);
    int fimlinha = leEspacos();

    if (!fimlinha){
        /*listar as carreiras*/
        listaCarreiras(*head);
        free(nome);
        return;
    }
    leNome(&nome);
    fimlinha = leEspacos();
    if (!fimlinha){
        if (!existeCarreira(nome, *head))
            criaCarreira(nome, head, tail);
        else{
            /*mostrar o percurso da carreira */
            percursoCarreira(nome, *head);
        }
    }
    else{
        char *inverso = malloc(sizeof(char) * MAXBUFFER);
        leNome(&inverso);
        if (!inversoCheck(inverso))
            printf("incorrect sort option.\n");
        else{
            /*mostrar o percurso inverso da carreira */
            percursoCarreiraInverso(nome, *head);
        leAteFinalLinha();
        free(inverso);
        }
    }
    free(nome);
}

/* Função que representa o comando 'l'-> adicionar ligações */
void adicionaLigacoes(Carreira **headC, Paragem **headP){
    char *carreira = malloc(sizeof(char) * MAXBUFFER), 
    *Porigem = malloc(sizeof(char) * MAXBUFFER),
    *Pdestino = malloc(sizeof(char) * MAXBUFFER);
    double custo= 0, duracao = 0;

    leNomesComando(&carreira,&Porigem,&Pdestino);
    if (scanf("%lf %lf", &custo, &duracao) == 2)
        leAteFinalLinha();

    if (!validateInput(carreira, Porigem, Pdestino, custo, duracao, headP, 
    headC)){
        free(carreira);
        free(Porigem);
        free(Pdestino);
        return;
    }
    
    adicionaLigacao(carreira,Porigem,Pdestino,*headC,custo,duracao,*headP);
    updateCarreirasporParagem(carreira,Porigem,*headP);
    updateCarreirasporParagem(carreira,Pdestino,*headP);
    free(carreira);
    free(Porigem);
    free(Pdestino);
}

/* Função que representa o comando 'i'-> listar interseções entre carreiras */
void listaIntersecoes(Paragem* head){
    Paragem* current = head;
    int i;

    if (head == NULL)
        return;
    
    while (current != NULL){
        if (current->num_carreiras > 1){
            ordenaCarreiras(current->carreiras,current->num_carreiras);
            printf("%s %d:", current->nome, current->num_carreiras);
            for (i = 0; i < current->num_carreiras; i++){
                printf(" %s", current->carreiras[i]);
            }
            printf("\n");
        }
        current = current->prox;
    }

}

/* Função que representa o comando 'r'-> remover uma carreira */
void removeCarreira(Carreira **headC, Carreira **tailC){
    char *carreira = malloc(sizeof(char) * MAXBUFFER);
    Carreira *current_carreira = *headC, *prev_carreira = NULL;

    leEspacos();
    leNome(&carreira);
    leAteFinalLinha();
    
    /* encontrar a carreira a ser removida */
    while (current_carreira != NULL && strcmp(current_carreira->nome, 
    carreira)) {
        prev_carreira = current_carreira;
        current_carreira = current_carreira->prox;
    }

    if (current_carreira == NULL) {
        printf("%s: no such line.\n", carreira);
        free(carreira);
        return;
    }

    /* update dos pointers da lista de carreiras  */
    if (prev_carreira == NULL) 
        *headC = current_carreira->prox; 
    else
        prev_carreira->prox = current_carreira->prox;
    if (current_carreira == *tailC) 
        *tailC = prev_carreira;
    
    /* remover carreira de todas as paragens e libertar as ligacoes da mesma */
    removeCarreiraFromParagens(current_carreira);

    /* free carreira */
    free(current_carreira->nome);
    free(current_carreira);
    free(carreira);
}

/* Função que representa o comando 'e'-> remover uma carreira */
void removeParagem(Paragem** headP, Paragem** tailP, Carreira* headC) {
    Paragem* current_paragem = *headP, *prev_paragem = NULL;
    char *paragem = malloc(sizeof(char) * MAXBUFFER);

    leEspacos();
    leNome(&paragem);
    leAteFinalLinha();

    /* encontrar a paragem que vai ser removida */
    while (current_paragem != NULL && strcmp(current_paragem->nome, 
    paragem)) {
        prev_paragem = current_paragem;
        current_paragem = current_paragem->prox;
    }
    if (current_paragem == NULL) {
        printf("%s: no such stop.\n", paragem);
        free(paragem);
        return;
    }
    /* iterar sobre o vetor de carreiras da paragem e atualizar as ligações */
    updateLigacaoListForCarreiras(current_paragem, headC);

    /* remover o nó da paragem da linked list de paragens e libertá-lo */
    if (prev_paragem == NULL) 
        *headP = current_paragem->prox;
    else 
        prev_paragem->prox = current_paragem->prox;
    if (current_paragem== *tailP)
        *tailP = prev_paragem;

    freeParagem(current_paragem);
    free(paragem); /* free no pointer que guardou o nome lido no input */
}

/* Função que representa o comando 'a'-> apagar os dados do sistema */
void apagaSistema(Paragem** headP, Carreira **headC, Paragem** tailP, Carreira 
**tailC){
    apagaParagensList(headP, tailP);
    apagaCarreirasList(headC, tailC);
}

/* -- main -- */

/* Lê o input e consoante o comando lido executa a função correta */
void handleCommand(char command, Paragem** headP, Paragem** tailP, 
Carreira** headC, Carreira** tailC) {
    switch (command) {
        case 'c':
            adiciona_e_lista_Carreiras(headC, tailC);
            break;
        case 'p':
            adiciona_e_lista_Paragens(headP, tailP);
            break;
        case 'l':
            adicionaLigacoes(headC, headP);
            break;
        case 'i':
            listaIntersecoes(*headP);
            break;
        case 'r':
            removeCarreira(headC, tailC);
            break;
        case 'e':
            removeParagem(headP, tailP, *headC);
            break;
        case 'a':
            apagaSistema(headP, headC, tailP, tailC);
            break;
        case 'q':
            break;
        default:
            /* Ignorar linhas em branco */
            if (command == ' ' || command == '\t' || command == '\n') break;
    }
}

/* Main */
int main(){
    int command;
    Paragem* headP = NULL, *tailP = NULL;
    Carreira* headC = NULL, *tailC = NULL;

    do {
        command = getchar();
        handleCommand(command, &headP, &tailP, &headC, &tailC);
    } while (command != 'q');
    apagaSistema(&headP, &headC, &tailP, &tailC);
    return 0;
}
