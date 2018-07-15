/*
Nome: Guilherme de Brito Abreu
RA: 173691

Objetivo: O objetivo deste programa é simular uma partida do jogo de cartas chamado Paciência;

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct List{
    char card[4];/*String que guarda as informacoes da carta*/
    char colorCard;/*Diferencia a cor da corta, sendo 0 para vermelho e 1 para preto*/
    char suitCard;/*Guarda o naipe da carta*/
    char valueCard;/*Guarda o valor da carta em relação as outras 12 cartas do baralho*/
    char state;/*Guarda o estado da carta, sendo 0 para carta fechada e 1 para aberta*/
    char changes;/*Guarda o numero de mudancas entre pilhas*/
    struct List *next;
};

typedef struct List List;

void inverte_lista(List *);/*Inverte uma pilha de cartas*/ 
void pickCards(List*);/*Preenche a pilha de estoque com as cartas do baralho*/
char worthCard(char *);/*Determina o valor da carta*/
void spreadCards(List *, List **);/*Distribui as cartas do baralho entre as pilhas de jogo*/
void heap(List*, List*, char);/*Empilha a primeira carta de uma pilha em outra pilha*/
void game(List *, List *, List **, List **);/*Simula o o jogo de paciencia*/
void closeProgram(List *, List *, List **, List **);/*Libera toda memoria que foi alocado durante o programa para a simulacao do jogo*/
List *beginList();/*Inicia uma lista com cabeça*/
int checkEmptyList(List *);/*Checa se uma lista está vazia e retorna 1 caso seja verdadeira a resposta*/
void insertTail(List *, char *, char, char);/*Insere um elemento no final de uma lista*/
void eraseList(List *);/*Exclui uma lista inteira com cabeça*/
void printList(List *);/*Imprimi as informações de uma lista*/
char getColor(char *);/*Define a cor da carta, sendo 0 para vermelha e 1 para preta*/
char getSuit(char *);/*Identifica qual é o naipe de uma carta*/
void game(List *, List *, List **, List **);/*Simula um jogo de paciencia*/
int endGame(List **);
void clearChanges(List **);


int main(){
    int i;
    
    List *J[7];/*J corresponde às 7 pilhas de jogo*/
    List *S[4];/*S corresponde às 4 pilhas de saída*/
    
    List *E = beginList();/*Inicia a pilha de estoque*/
    List *D = beginList();/*Inicia a pilha de descarte*/

    i = 0;
    while(i<7){
        /*Inicia as 7 pilhas do jogo*/
        J[i] = beginList();
        i++;
    }
    
    i = 0;
    while(i<4){
        /*Inicia as 7 pilhas de saída*/
        S[i] = beginList();
        i++;
    }

    pickCards(E);/*Retira as cartas do baralho e as coloca no pilha de estoque*/
    
    spreadCards(E,J);/*Distribui as cartas para as pilhas do jogo*/

    inverte_lista(E);/*Inverte a pilha de estoque para ficar na ordem desejada*/
    
    game(E,D,J,S);/*Simula o jogo*/
    
    /*
    *Este conjunto de comandos imprimem as pilhas após a simulação do jogo.*/
    printf("\n");
    printf("E ");printList(E);
    printf("D ");printList(D);

    for(i=0; i < 7; i++){
        printf("J%d ", i+1);printList(J[i]);
    }
    for(i=0; i < 4; i++){
        printf("S%d ", i+1);printList(S[i]);
    }

    closeProgram(E,D,J,S);/*Libera toda a memória utilizada antes de fechar o programa*/
    return 0;
}

void inverte_lista(List *cabeca) {
    List *a, *b, *c;
    a = NULL;
    b = cabeca->next;
    while (b != NULL) {
        c = b->next;
        b->next = a;
        a = b;
        b = c;
    }
    cabeca->next = a;
}

void closeProgram(List *E, List *D, List **J, List **S){
    int i;

    for(i=0;i<7;i++){
        eraseList(J[i]);
    }
    for(i=0;i<4;i++){
        eraseList(S[i]);
    }
    
    eraseList(E);
    eraseList(D);

    return;
}

void game(List *E, List *D, List **J, List **S){
    char step = 1, end = 0, jump = 0;
    int i,j,moves;
    List *aux, *sentinela, *helper;
    
    
    moves = 0;
    while(!end){
        switch(step){
            case 1:
            /*Caso a pilha de descarte esteja vazia e a pilha de estoque não, o program deve empilha uma carta do estoque na pilha de descarte*/
                if (checkEmptyList(D) && !checkEmptyList(E)){
                    heap(E,D,1);
                    printf("E D %s\n", D->next->card);
		    moves++;
                }
            case 2:
            /*Caso tenha alguma pilha de jogo sem cartas abertas o programa deve abrir carta do topo*/
                i = 0;
                while(i<7){
                    if (!checkEmptyList(J[i])){
                        if (J[i]->next->state == 0){
                            J[i]->next->state = 1;
                            printf("J%d J%d %s\n", i+1,i+1,J[i]->next->card);
			    moves++;
                        }
                    }
                    i++;
                }
            case 3:
            /*Caso a carta do topo de uma pilha de jogo seja um ás o programa deverá colocá-la na pilha de saída corresponde a carta*/
                i = 0;
                while(i<7){
                    if (!checkEmptyList(J[i])){
                        if (J[i]->next->valueCard == 1){
                            switch(J[i]->next->suitCard){
                                case 'O':
                                    heap(J[i],S[0],1);
                                    printf("J%d S1 %s\n",i+1, S[0]->next->card);
                                    break;
                                case 'C':
                                    heap(J[i],S[1],1);
                                    printf("J%d S2 %s\n",i+1, S[1]->next->card);
                                    break;
                                case 'P':
                                    heap(J[i],S[2],1);
                                    printf("J%d S3 %s\n",i+1, S[2]->next->card);
                                    break;
                                case 'E':
                                    heap(J[i],S[3],1);
                                    printf("J%d S4 %s\n",i+1, S[3]->next->card);
                                    break;
                            }
                            jump = 1;
                        }
                    }
                    i++;
                    if(jump)
			break;
                }
                if(jump){
                /*Caso o programa tenha retirado ao menos uma carta de uma pilha de jogo o programa deverá voltar ao caso 2*/
                    jump = 0;
                    step = 2;
                    moves++;
		    break;
                }
            case 4:
            /*Caso a carta do topo de uma pilha de jogo seja a consecutiva da carta que está em uma pilha de saída o programa deverá colocá-la lá*/
                for(i = 0; i < 7; i++){
                    for(j = 0; j < 4; j++){
                        if(!checkEmptyList(J[i]) && !checkEmptyList(S[j])){
                            if (((J[i]->next->valueCard - S[j]->next->valueCard) == 1) && (J[i]->next->suitCard == S[j]->next->suitCard)){
                                heap(J[i],S[j],1);
                                printf("J%d S%d %s\n",i+1,j+1, S[j]->next->card);
                                jump = 1;
                                break;
                            }
                        }
                    }
                    if(jump)
                        break;
                }
                if(jump){
                    /*Caso o programa tenha retirado ao menos uma carta de uma pilha de jogo o programa deverá voltar ao caso 2*/
                    jump = 0;
                    step = 2;
		    moves++;
                    break;
                }
            case 5:
            /*Caso tenha alguma carta ou conjunto de cartas que possam ser movidas para outra pilha de jogo o programa as move*/
            /*O programa apenas as move caso o numero de mudanças entre pilhas da carta que está na base do conjunto de cartas seja menor que 2*/
                for(i=0; i < 7; i++){
                    sentinela = NULL;
                    aux = J[i]->next;
                    while(aux != NULL){
                        if(aux->state == 1){
                            sentinela = aux;
                        }
                        else{
                            break;
                        }
                        aux = aux->next;
                    }
                    if(sentinela!=NULL){
                        for(j = 0; j < 7; j++){
                            if(!checkEmptyList(J[j])){
                                if (J[j]->next->state == 1){
                                    if (((J[j]->next->valueCard - sentinela->valueCard) == 1) && (J[j]->next->colorCard != sentinela->colorCard) && (sentinela->changes < 2)){
                                        sentinela->changes++;
                                        
                                        helper = beginList();
                                        while(helper->next != sentinela)
                                            heap(J[i],helper,1);
                                        
                                        printf("J%d J%d",i+1,j+1);
                                        printList(helper);
                                        
                                        while(helper->next!=NULL)    
                                            heap(helper,J[j],1);
                                        
                                        eraseList(helper);
                                        jump = 1;
                                        break;
                                    }
                                }
                            }
                            else{
                                if(sentinela->valueCard == 13 && (sentinela->changes < 2)){
                                    sentinela->changes++;
                                    
                                    helper = beginList();
                                    while(helper->next != sentinela)
                                        heap(J[i],helper,1);
                                    
                                    printf("J%d J%d",i+1,j+1);
                                    printList(helper);
                                    
                                    while(helper->next!=NULL)    
                                        heap(helper,J[j],1);
                                    
                                    eraseList(helper);
                                    jump = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if(jump){
                        break;
                    }
                }
                if(jump){
                    /*Caso o programa tenha retirado ao menos uma carta de uma pilha de jogo o programa deverá voltar ao caso 2*/
                    jump = 0;
                    step = 2;
		    moves++;
                    break;
                }
            case 6:
                /*Caso a carta que esteja no topo da pilha de descarte esteja apta a ser movida o programa a move*/
                if(!checkEmptyList(D)){
                    for(j = 0; j < 4; j++){
                        if(!checkEmptyList(S[j])){
                            if (((S[j]->next->valueCard - D->next->valueCard) == -1) && (S[j]->next->suitCard == D->next->suitCard)){             
                                heap(D,S[j],1);
                                printf("D S%d %s\n",j+1, S[j]->next->card);
                                jump = 1;
                                break;
                            }
                        }
                        else{
                            if ((D->next->valueCard == 1)){
                                switch(D->next->suitCard){
                                    case 'O':
                                        heap(D,S[0],1);
                                        printf("D S1 %s\n", S[0]->next->card);
                                        break;
                                    case 'C':
                                        heap(D,S[1],1);
                                        printf("D S2 %s\n", S[1]->next->card);
                                        break;
                                    case 'P':
                                        heap(D,S[2],1);
                                        printf("D S3 %s\n", S[2]->next->card);
                                        break;
                                    case 'E':
                                        heap(D,S[3],1);
                                        printf("D S4 %s\n", S[3]->next->card);
                                        break;  
                                }
                                jump = 1;
                                break;
                            }
                        }
                        if(jump)
                            break;
                    }
                    if(jump){
                        /*Caso o programa tenha retirado uma carta da pilha de descarte o programa deverá voltar ao caso 1*/
                        jump = 0;
                        step = 1;
			moves++;
                        break;
                    }
                }
                if (!checkEmptyList(D)){
                    for(j = 0; j < 7; j++){
                        if(!checkEmptyList(J[j])){
                            if (((J[j]->next->valueCard - D->next->valueCard) == 1) && (J[j]->next->colorCard != D->next->colorCard)){             
                                heap(D,J[j],1);
                                printf("D J%d %s\n",j+1, J[j]->next->card);
                                jump = 1;
                                break;
                            }
                        }
                        else{
                            if (D->next->valueCard == 13){
                                heap(D,J[j],1);
                                printf("D J%d %s\n",j+1, J[j]->next->card);
                                jump = 1;
                                break;
                            }
                        }
                    }
                }
                if(jump){
                    /*Caso o programa tenha retirado uma carta da pilha de descarte o programa deverá voltar ao caso 1*/
                    jump = 0;
                    step = 1;
		    moves++;
                    break;
                }
            case 7:
            /*Caso a pilha de estoque esteja vazia o jogo deve acabar, pois não há mais jogadas válidas*/
            /*Caso contrário o programa deverá empilhar uma carta na pilha de descarte e voltar ao passo 6*/
                if(endGame(S)){
		    printf("FIM\n");
                    end = 1;
                }
                else if(checkEmptyList(E) && moves < 1000){
		    clearChanges(J);
		    step = 2;
		}
                else if (!checkEmptyList(E)){
                    heap(E,D,1);
                    printf("E D %s\n", D->next->card);
                    step = 6;
                }
                else{
		    printf("FIM\n");
                    end = 1;
		}
                break;
        }
    }
}

int endGame(List **S){
    int i,a = 1;
    
    for(i=0; i < 4 && a; i++){
	if (!checkEmptyList(S[i])){
	    if (!(S[i]->next->valueCard == 13))
		a = 0;
	}
	else{
	    a = 0;
	}
    }
    
    return a;
}


void clearChanges(List **J){
    List *p;
    int i;
    
    for(i=0; i < 7; i++){
	p = J[i]->next;
	while(p){
	    if(p->changes == 2)
		p->changes = 0;
	    p = p->next;
	}
    }
      
      
    return;
}


List *beginList(){
    List *head = (List*)malloc(sizeof(List));
    head->next = NULL;
    return head;
}

int checkEmptyList(List *head){
    return (head->next == NULL);
}


void insertTail(List *head, char *card, char valueCard, char state){
    List *helper = head;
    List *new;

    if(helper != NULL){
        while(helper->next != NULL){
            helper = helper->next;
        }

        new = (List*)malloc(sizeof(List));
        strcpy(new->card, card);
        new->colorCard = getColor(card);
        new->suitCard = getSuit(card);
        new->valueCard = valueCard;
        new->state = state;
        new->changes = 0;
        helper->next = new;
        new->next = NULL;
    }

    return;
}

char getColor(char *card){
    char c;

    if (card[1] != '0'){
        switch(card[1]){
            case 'O':
            case 'C':
                c = 0;
                break;
            
            case 'P':
            case 'E':
                c = 1;
                break;
        }
    }
    else{
        switch(card[2]){
            case 'O':
            case 'C':
                c = 0;
                break;
            
            case 'P':
            case 'E':
                c = 1;
                break;
        }
    }

    return c;
}

char getSuit(char *card){
    if (card[1] != '0'){
        return card[1];
    }
    else{
        return card[2];
    }
}

void eraseList(List *head){
    List *helper;
    
    if (head != NULL){
        while(head->next != NULL){
            helper = head->next;
            head->next = head->next->next;
            free(helper);
        }       
        free(head);
    }
    return;
}

void printList(List *head){
    if(head!=NULL){
        while(head->next != NULL){
            printf(" %s", head->next->card);
            head = head->next;
        }
        printf("\n");
    }
    return;
}


void pickCards(List *E){
    char i = 0;
    char helper2, helper1[4];
    
    while(i<52){
        scanf("%s", helper1);
        helper2 = worthCard(helper1);
        insertTail(E, helper1, helper2, 0);
        i++;
    }
    
    return;
}

char worthCard(char *helper1){
    char helper2;
    
    if (helper1[0]=='A')
        helper2 = 1;
    else if (helper1[0]=='J')
        helper2 = 11;
    else if (helper1[0]=='Q')
        helper2 = 12;
    else if (helper1[0]=='K')
        helper2 = 13;
    else if (helper1[0]=='1')
        helper2 = 10;
    else
        helper2 = helper1[0] - 48;

    return helper2;
}

void spreadCards(List *E, List **J){
    int i,j;
    
    for(i = 6; i > -1; i--){
        for(j = 6 - i; j < 7; j++){
            heap(E,J[j],0);
        }
    }
    
    return;
}

void heap(List *E, List *J, char state){
    List *helper;

    if (E->next != NULL){
        helper = J->next;
        J->next = E->next;
        J->next->state = state;
        E->next = E->next->next;
        J->next->next = helper;
    }
    
    return;
}