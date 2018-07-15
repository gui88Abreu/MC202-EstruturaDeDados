/*
NOME: GUILHERME DE BRITO ABREU
RA: 173691

Descricao do programa: Este programa basicamente simula o jogo "Oracle of Bacon". O jogo consiste
em achar atraves de uma banco de dados o menor grau de distancia entre qualquer ator ate Kevin Bacon.
Neste banco de dados esta contido o nome de varios atores e os filmes que cada um ja trabalhou em sua
carreira. Assim, temos apenas que encontrar qual eh a menor distancia entre dois atores analisando 
todos os filmes onde ambos trabalharam e onde os atores que trabalharam nesses filmes tambem trabalha-
ram. No caso deste programa, queremos achar o caminho minimo entre quaisquer atores e nao apenas ate 
Kevin Bacon. 


Descricao do Codigo: Para esta implementacao foram utilizadas 4 estruturas de dados: Grafo, Lista Ligada
Fila e HashTable. O Grafo foi utilizado para estabelecer uma relacao clara entre os atores e os filmes onde
eles trabalharam, e vice-versa. O Hashtable foi utilizado para que o programa tivesse um rendimento maior,
ja que para esta aplicacao o tempo de cada operacao eh aproximadamente 1. As listas ligadas foram essencias,
ja que um no de grafo possui uma lista de adjacencias, e para tratar dos casos de colisao do Hashtable tambem
eh necessario utilizar uma estrutura de dados auxiliar, que no caso tambem foi uma lista ligada. E por ultimo,
a fila eh esssencial para o algoritmo de busca em largura em um grafo, portanto, tambem foi necessaria a sua
utilizacao.

*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define M 1299709

typedef enum _cores {branco, cinza, preto} cores;

typedef struct _lista* list;

typedef struct _queue{
/*Estrutura de dados para armazenar uma fila de listas*/
	list l;
	struct _queue *prox, *ant;
}myQueue,*queue;

typedef struct _listaAdj{
/*Estrutura de dados para armazenar as adjacencias do no de um grafo*/
	list l;
	struct _listaAdj *prox;
}ListaAdj, *lstAdj;

typedef struct _lista{
/*ESTRUTURA DE DADOS PARA UMA LISTA LIGADA*/
	char *name;
	lstAdj adj;/*ponteiro para a lista de adjacencias do no*/
	struct _lista *prox;/*ponteiro para o proximo elemento da lista de onde o no esta inserido*/
	struct _lista *ant;/*ponterio para o predecessor, utilizado na busca*/
	cores cor;
}Lista;

typedef struct _hashTable{
/*ESTRUTURA DE DADOS PARA UM HASHTABLE*/
	list l;
}hashTable, *table;

/*Conjunto de funcoes para o hashTable*/
unsigned long int hash(char *, unsigned long int );/*CALCULA HASH DE UMA STRING E RETORNA O SEU VALOR*/
list add_Table(int , table *, char*);/*ADICIONA UM ELEMENTO NA TABELA*/
table TableMemory();/*CRIA UM ESPACO DE MEMORIA PARA UMA POSICAO DE UMA TABELA E RETORNA O SEU ENDERECO*/
list ListMemory();/*CRIA UM ESPACO DE MEMORIA PARA UMA POSICAO DE UMA LISTA E RETORNA O SEU ENDERECO*/
list appendList(table , char *);/*INSERE UM NOVO ELEMENTO NUME LISTA*/
list search(char*, table*, int , int *);/*PROCURA POR UM ELEMENTO DA TABELA E RETORNA O ENDERECO DE MEMORIA CASO TENHA ACHADO E NULL CASO CONTRARIO*/
void printInfo(list);/*IMPRIMI AS INFORMACOES DE UM ELEMENTO DA TABELA*/
void eraselist(list);/*DESALOCA A MEMORIA DE UMA LISTA INTEIRA*/
void eraseTable(table *, int );/*DESALOCA A MEMORIA DE UMA TABELA INTEIRA*/

/*Conjunto de funcoes para o Grafo*/
void appendLstAdj(list, list);/*adiciona elementtos em uma lista de adjacencias*/
void eraseLstAdj(lstAdj );/*desaloca toda memoria de uma lista de adjacencias*/
void breadthFirstSearch(table *, char *, char *);

/*Conjunto de funcoes para a fila*/
queue queueMemory();/*Cria um espaco de memoria para um no de uma fila que armazena listas*/
void eraseQueue(queue );/*Desaloca toda memoria alocada em uma fila*/
list dequeue(queue );/*Remove um elemento da fila passada como parametro*/
void enqueue(queue , list);/*Insere um novo elemento na fila passada como parametro*/

/*Conjunto de funcoes auxiliares*/
void addFolks(table *, char **);/*adiciona todo o pessoal na tabela de hash*/
void readFileIN(table *, char *);/*le arquivo de entrada com os dados dos atores*/
void query(table *, char *);/*Consulta os dados dos atores atraves de arquivo dado como entrada*/
void printPath(char *, table *);/*Imprimmi o caminho de um vertice a outro no grafo*/
char *myStrdup(const char *);/*Retorna a copia de uma string*/

/*Qualquer trecho de codigo que nao estiver comentado, eh possivel interpreta-lo atraves do nome das variaveis... THANKS*/

int main(int argc, char *argv[]){
	table *t = (table*)calloc(M,sizeof(table));

	readFileIN(t, argv[1]);/*Le arquivo*/
	query(t,argv[2]);/*Faz as consultas*/
	
	eraseTable(t,M);/*Apaga toda memoria para fechar o programa*/
	return 0;
}

void query(table *t, char *arqQuery){
	FILE *arq = fopen(arqQuery,"r");
	char reading[2][350], *actor1, *actor2;
	int str;
	char *pegaFgets;

	if(!arq)
		printf("Erro na abertura do arquivo %s\n", arqQuery);

	while(fgets(reading[0], 349, arq)){
		actor1 = strtok(reading[0],"\t\n");
		str = strlen(actor1)-1;
		if(actor1[str] == 13)/*Corrigi a insercao de um \n*/
			actor1[str] = '\0';
		
		pegaFgets = fgets(reading[1], 349, arq);

		actor2 = strtok(reading[1],"\t\n");
		str = strlen(actor2)-1;
		if(actor2[str] == 13)/*Corrigi a insercao de um \n*/
			actor2[str] = '\0';

		breadthFirstSearch(t,actor2,actor1);/*Busca do ator1 ate o ator2\n*/
		printPath(actor1,t);/*Imprimi o caminho a partir do ator1*/
		
	}

	fclose(arq);
	return;
}

void printPath(char *actor, table *t){
	list aux = search(actor,t,M,NULL);

	if(aux && (aux->ant)){
		printf("%s\n", actor);
		aux = aux->ant;
		while(aux){
			printf("trabalhou em %c%s%c com\n",34,aux->name,34);
			aux = aux->ant;
			printf("%s\n", aux->name);
			aux = aux->ant;
		}
	}
	else{
		printf("CAMINHO NAO ENCONTRADO\n");
	}

	return;
}

void breadthFirstSearch(table *t, char *actor1, char *actor2){
	long int i = 0;
	list aux;
	lstAdj helper;
	queue Q = queueMemory();

	while(i<M){
	/*Inicializa todas as informacoes necessarias para a busca em largura*/
		if(t[i]){
			aux = t[i]->l->prox;
			while(aux){
				aux->cor = branco;
				aux->ant = NULL;
				if (strcmp(aux->name,actor1)==0){
					aux->cor = cinza;
					enqueue(Q,aux);
				}
				aux = aux->prox;
			}
		}
		i++;
	}

	while(Q->prox){
		aux = dequeue(Q);
		aux->cor = preto;
		helper = aux->adj;
		while(helper){
			if(helper->l->cor == branco){
				helper->l->ant = aux;
				helper->l->cor = cinza;
				if(strcmp(helper->l->name,actor2) == 0){
				/*Caso tenha chegado no noh desejado, a busca ja pode parar*/
					eraseQueue(Q);
					return;
				}
				enqueue(Q,helper->l);
			}
			helper = helper->prox;
		}
	}

	eraseQueue(Q);
	return;
}

void eraseQueue(queue Q){
	if(Q){
		while(Q->prox)
			dequeue(Q);
		free(Q);
	}
	return;
}

queue queueMemory(){
	queue new = (queue)malloc(sizeof(myQueue));
	new->prox = NULL;
	new->ant = NULL;
	return new;
}

list dequeue(queue Q){
	list ret = NULL;
	queue aux;
	if(Q->ant){
		ret = Q->ant->l;
		aux = Q->ant;
		Q->ant = aux->ant;
		if(Q->ant==Q)
			Q->ant = NULL;
		free(aux);
		aux = Q->ant;
		if(aux)
			aux->prox = NULL;
		else
			Q->prox = NULL;
	}
	return ret;
}

void enqueue(queue Q, list ele){
	queue new = queueMemory();
	queue aux = Q->prox;
	
	new->l = ele;
	new->prox = aux;
	Q->prox = new;
	new->ant = Q;
	if(Q->ant==NULL)
		Q->ant = new;
	else{
		aux->ant = new;
	}
	return;
}

void readFileIN(table *t, char *arqIn){
	FILE *arq = fopen(arqIn,"r");
	char line[700], **reading;
	reading = (char**)malloc(2*sizeof(char*));

	if(arq==NULL){
		printf("Erro na abertura do arquivo: %s\n", arqIn);
		exit(1);
	}

	while(fgets(line,699,arq)){
		reading[0] = strtok(line,"\t");/*Nome do ator*/
		reading[1] = strtok(NULL,"\t\n");/*Nome do filme*/
		addFolks(t,reading);/*Adiciona-os ao hashtable*/
	}

	free(reading);
	fclose(arq);
	return;
}

void addFolks(table *t, char **reading){
	int h1,h2;
	
	list aux1 = search(reading[0], t, M, &h1);
	list aux2 = search(reading[1], t, M, &h2);

	if(aux1 && aux2){
	/*Caso onde o ator ja esta inserido e o filme tambem*/
		appendLstAdj(aux1,aux2);
		appendLstAdj(aux2,aux1);
	}
	else if(aux1 && !aux2){
	/*Caso onde o ator ja esta inserido e o filme nao*/
		aux2 = add_Table(h2, t, reading[1]);
		appendLstAdj(aux1,aux2);
		appendLstAdj(aux2,aux1);
	}
	else if(!aux1 && !aux2){
	/*Caso onde o ator e o filme nao estao inseridos*/
		aux1 = add_Table(h1, t, reading[0]);
		aux2 = add_Table(h2, t, reading[1]);
		appendLstAdj(aux1,aux2);
		appendLstAdj(aux2,aux1);
	}
	else{
	/*Caso onde o ator nao esta inserido mas o filme sim*/
		aux1 = add_Table(h1, t, reading[0]);
		appendLstAdj(aux1,aux2);
		appendLstAdj(aux2,aux1);
	}

	return;
}

void eraseLstAdj(lstAdj x){
	lstAdj aux;
	if(x!=NULL){
		while(x->prox!=NULL){
			aux = x->prox;
			x->prox = aux->prox;
			free(aux);
		}
		free(x);
	}
	return;
}

lstAdj lstAdjMemory(){
	lstAdj new = (lstAdj)malloc(sizeof(ListaAdj));
	new->l = NULL;
	new->prox = NULL;
	return new;
}

void appendLstAdj(list n, list k){
	lstAdj aux1 = lstAdjMemory();
	aux1->l = k;
	aux1->prox = n->adj;
	n->adj = aux1;
	return;
}

unsigned long int hash(char *reading, unsigned long int N){
	unsigned long long int key = 0, str = strlen(reading);
	int i;
	
	for(i = 0; i < str; i++){
	/*Calcula o hash atraves da posicao de cada caracter na string*/
		key = key%N + (unsigned long long int)pow(sqrt(reading[i]),i)%N;
	}

	key = key%N;
	return key;
}

list add_Table(int h, table *t, char *reading){
	list ret;

	if(t[h]){
		ret = appendList(t[h],reading);
	}
	else{
		t[h] = TableMemory();
		ret = appendList(t[h],reading);
	}
	return ret;
}

table TableMemory(){
	table new = (table)malloc(sizeof(hashTable));
	new->l = ListMemory();
	return new;
}

list ListMemory(){
	list new = (list)malloc(sizeof(Lista));
	new->adj = NULL;
	new->prox = NULL;
	new->ant = NULL;
	return new;
}

char *myStrdup(const char *reading){
	char *dup = (char*)malloc(strlen(reading) + 1);
	if(dup)
		strcpy(dup,reading);
	return dup;
}

list appendList(table t, char *reading){
	list new = ListMemory();
	new->name = myStrdup(reading);
	new->prox = t->l->prox;
	t->l->prox = new;
	return new;
}

list search(char *reading, table *t, int N, int *h){
	list current;
	int H;

	if(h){
	/*Caso onde e necessario retornar o valor do hash*/
		*h = hash(reading,N);
		H = *h;
	}
	else
	/*Caso onde nao e necessario retornar o valor do hash*/
		H = hash(reading,N);


	if(t[H]){
		current = t[H]->l->prox;
		while(current!=NULL && !(strcmp(current->name,reading)==0)){
			current = current->prox;
		}
	}
	else{
		current = NULL;
	}

	return current;
}

void eraselist(list x){
	list aux;
	if(x!=NULL){
		while(x->prox!=NULL){
			aux = x->prox;
			x->prox = aux->prox;
			eraseLstAdj(aux->adj);
			free(aux->name);
			free(aux);
		}
		free(x);
	}
	return;
}

void removeElement(table e){
	if(e){
		eraselist(e->l);
		free(e);
	}
	return;
}

void eraseTable(table *t, int N){
	int i;

	for(i = 0; i < N; i++){
		removeElement(t[i]);
	}

	free(t);
}